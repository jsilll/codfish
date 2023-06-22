#include <codfish/uci.hpp>

#include <algorithm>
#include <chrono>
#include <climits>
#include <future>
#include <iostream>
#include <optional>

#include <codfish/time.hpp>

#define DEFAULT_MIN_DEPTH 8
#define DEFAULT_MAX_DEPTH 32
#define WINDOW_EXPANSION  50
#define MIN_EVAL          (INT_MIN + 1)

namespace codfish::uci {
struct UCIScore {
    bool is_mate;
    std::string repr;
};

static UCIScore
score_to_uci(int score) {
    if (score >= 0) {
        int check_mate_delta = (-MIN_EVAL) - score;
        int moves_for_mate = (check_mate_delta / 2) + (check_mate_delta % 2);
        if (0 < moves_for_mate and moves_for_mate <= 16) {
            return UCIScore{true, "mate " + std::to_string(moves_for_mate)};
        }
    } else {
        int check_mate_delta = -(MIN_EVAL - score);
        int moves_for_mate = (check_mate_delta / 2) + (check_mate_delta % 2);
        if (0 < moves_for_mate and moves_for_mate <= 16) {
            return UCIScore{true, "mate " + std::to_string(-moves_for_mate)};
        }
    }

    return UCIScore{false, "cp " + std::to_string(score)};
}

static bool
display_search_iteration(codbrain::Result result, int depth,
                         std::chrono::duration<double> elapsed) {
    UCIScore score = score_to_uci(result.score);
    std::cout << "info score " << score.repr << " depth " << depth << " nodes "
              << result.nodes << " time "
              << (int) (elapsed / std::chrono::milliseconds(1)) << " pv ";

    std::for_each(result.pv.cbegin(), result.pv.cend(),
                  [](const codchess::Move &move) {
                      std::cout << move.ToString() << " ";
                  });
    std::cout << std::endl;

    return score.is_mate;
}

static void
Search(std::future<void> future, codbrain::Brain &move_picker,
       codbrain::Result &result) {
    int alpha = MIN_EVAL;
    int beta = -MIN_EVAL;

    move_picker.ClearMoveTables();

    for (std::uint32_t depth = 1; depth <= move_picker.max_depth(); ++depth) {
        auto start = std::chrono::system_clock::now();
        result = move_picker.FindBestMove(depth, alpha, beta);
        auto end = std::chrono::system_clock::now();

        bool found_mate = display_search_iteration(result, depth, end - start);

        if (found_mate) {
            break;
        }

        if (result.nodes == 2) {
            break;
        }

        if (future.wait_for(std::chrono::milliseconds(1)) ==
            std::future_status::ready) {
            break;
        }

        if ((result.score <= alpha) or (result.score >= beta)) {
            alpha = MIN_EVAL;
            beta = -MIN_EVAL;
            depth--;
            continue;
        }

        alpha = result.score - WINDOW_EXPANSION;
        beta = result.score + WINDOW_EXPANSION;
    }
}

void
Go(repl::State &state, const repl::Tokens &args) {
    enum ArgCommand : int {
        DEPTH,
        WTIME,
        BTIME,
        MOVES_TO_GO,
    };

    ArgCommand arg_cmd{};

    int depth{};
    int wtime{}, btime{};
    bool infinite = false;
    [[maybe_unused]] int moves_to_go{};
    for (const std::string &token : args) {
        if (token == "depth") {
            arg_cmd = DEPTH;
        } else if (token == "wtime") {
            arg_cmd = WTIME;
        } else if (token == "btime") {
            arg_cmd = BTIME;
        } else if (token == "movestogo") {
            arg_cmd = MOVES_TO_GO;
        } else if (token == "infinite") {
            infinite = true;
        } else {
            int val{};
            try {
                val = std::stoi(token);
            } catch (const std::exception &e) {
                return;
            }

            if (val < 0) {
                return;
            }

            switch (arg_cmd) {
            case DEPTH:
                if (val < 0) {
                    return;
                } else {
                    depth = val;
                }
                break;
            case WTIME:
                if (val <= 0) {
                    return;
                } else {
                    wtime = val;
                }
                break;
            case BTIME:
                if (val <= 0) {
                    return;
                } else {
                    btime = val;
                }
                break;
            case MOVES_TO_GO:
                if (val <= 0) {
                    return;
                } else {
                    moves_to_go = val;
                }
                break;
            }
        }
    }

    codbrain::Result result;
    if (!depth and !infinite and wtime and btime) {
        state.brain.max_depth(DEFAULT_MAX_DEPTH);
    } else {
        state.brain.max_depth(DEFAULT_MIN_DEPTH);
    }

    std::promise<void> signal_exit;
    std::future<void> signal_exit_future = signal_exit.get_future();
    std::thread search_thread(Search, std::move(signal_exit_future),
                              std::ref(state.brain), std::ref(result));
    if ((state.brain.board().active() == codchess::WHITE and wtime) or
        (state.brain.board().active() == codchess::BLACK and btime)) {
        std::this_thread::sleep_for(std::chrono::milliseconds(
            time::ComputeTimeBudgetMs(wtime, btime, state.brain.board())));
        signal_exit.set_value();
    }
    search_thread.join();

    std::cout << "bestmove " << result.pv[0].ToString() << std::endl;
}

// ---------------------------------------------------------------------------

static std::optional<codchess::Move>
ParseMove(std::string move_uci, codchess::Board &board) {
    for (const auto move : codchess::movegen::Legal(board)) {
        if (move.ToString() == move_uci) {
            return move;
        }
    }

    return std::nullopt;
}

static void
HandleMoves(std::vector<std::string> &moves, codchess::Board &board,
            codbrain::Brain &move_picker) {
    move_picker.AddToHistory(board.hash());   // For Three-Fold Draws
    for (std::string move_uci : moves) {
        const auto parsed_move = ParseMove(move_uci, board);
        if (parsed_move.has_value()) {
            const auto move = parsed_move.value();
            board.Make(move);
            move_picker.AddToHistory(board.hash());   // For Three-Fold Draws
        }
    }
}

void
Position(repl::State &state, repl::Tokens &args) {
    if (args.size() == 0) {
        return;
    }

    if (args[0] == "startpos") {
        args.erase(args.begin());

        state.brain.board().SetStartingPosition();
    } else if (args[0] == "fen") {
        args.erase(args.begin());

        if (args.size() < 6) {
            return;
        }

        std::vector<std::string> fen_string(args.begin(), args.begin() + 6);
        std::string fen_string_concat{};
        std::for_each(fen_string.cbegin(), fen_string.cend(),
                      [&fen_string_concat](const std::string &token) {
                          fen_string_concat += token + " ";
                      });
        if (!codchess::utils::ValidFen(fen_string_concat)) {
            return;
        } else {
            state.brain.board().FromFen(fen_string_concat);
            args.erase(args.begin(), args.begin() + 6);
        }
    } else {
        return;
    }

    if (args.size() == 0) {
        return;
    }

    state.brain.ClearHistory();

    if (args[0] == "moves") {
        args.erase(args.begin());

        HandleMoves(args, state.brain.board(), state.brain);
    }

    state.brain.ClearTTable();
    state.brain.ClearMoveTables();
}
}   // namespace codfish::uci