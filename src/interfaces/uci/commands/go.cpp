#include <interfaces/uci/commands/commands.hpp>

#include <interfaces/uci/timectl.hpp>

#include <engine/movepicker/movepicker.hpp>

#include <algorithm>
#include <chrono>
#include <climits>
#include <future>
#include <iostream>

#define DEFAULT_MIN_DEPTH 8
#define DEFAULT_MAX_DEPTH 32
#define MIN_EVAL (INT_MIN + 1)
#define WINDOW_EXPANSION 50

struct UCIScore
{
    std::string repr;
    bool is_mate;
};

static UCIScore score_to_uci(int score)
{
    if (score >= 0)
    {
        int check_mate_delta = (-MIN_EVAL) - score;
        int moves_for_mate = (check_mate_delta / 2) + (check_mate_delta % 2);
        if (0 < moves_for_mate && moves_for_mate <= 16)
        {
            return UCIScore{std::string("mate ") + std::to_string(moves_for_mate), true};
        }
    }
    else
    {
        int check_mate_delta = -(MIN_EVAL - score);
        int moves_for_mate = (check_mate_delta / 2) + (check_mate_delta % 2);
        if (0 < moves_for_mate && moves_for_mate <= 16)
        {
            return UCIScore{std::string("mate ") + std::to_string(-moves_for_mate), true};
        }
    }

    return UCIScore{std::string("cp ") + std::to_string(score), false};
}

/**
 * @brief Displays all the necessary info about current iteration
 * If score corresponds to a mate in x moves, returns true
 *
 * @param result
 * @param depth
 * @param elapsed
 */
static bool display_search_iteration(MovePicker::SearchResult result, int depth, std::chrono::duration<double> elapsed)
{
    UCIScore score = score_to_uci(result.score);
    std::cout << "info score " << score.repr
              << " depth " << depth
              << " nodes " << result.nodes
              << " time " << (int)(elapsed / std::chrono::milliseconds(1))
              << " pv ";

    std::for_each(result.pv.cbegin(), result.pv.cend(), [](const Move &move)
                  { std::cout << move.get_uci() << " "; });
    std::cout << std::endl;

    return score.is_mate;
}

static void search(std::future<void> future, MovePicker &move_picker, MovePicker::SearchResult &result)
{
    int alpha = MIN_EVAL;
    int beta = -MIN_EVAL;

    move_picker.clear_move_tables();

    for (int depth = 1; depth <= move_picker.get_max_depth(); depth++)
    {
        auto start = std::chrono::system_clock::now();
        result = move_picker.find_best_move(depth, alpha, beta);
        auto end = std::chrono::system_clock::now();

        bool found_mate = display_search_iteration(result, depth, end - start);

        if (found_mate)
        {
            break;
        }

        if (result.nodes == 2)
        {
            break;
        }

        if (future.wait_for(std::chrono::milliseconds(1)) == std::future_status::ready)
        {
            break;
        }

        if ((result.score <= alpha) || (result.score >= beta))
        {
            alpha = MIN_EVAL;
            beta = -MIN_EVAL;
            depth--;
            continue;
        }

        alpha = result.score - WINDOW_EXPANSION;
        beta = result.score + WINDOW_EXPANSION;
    }
}

void uci::GoCommand::execute(std::vector<std::string> &args)
{
    enum ArgCommand : int
    {
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
    for (const std::string &token : args)
    {
        if (token == "depth")
        {
            arg_cmd = DEPTH;
        }
        else if (token == "wtime")
        {
            arg_cmd = WTIME;
        }
        else if (token == "btime")
        {
            arg_cmd = BTIME;
        }
        else if (token == "movestogo")
        {
            arg_cmd = MOVES_TO_GO;
        }
        else if (token == "infinite")
        {
            infinite = true;
        }
        else
        {
            int val{};
            try
            {
                val = std::stoi(token);
            }
            catch (const std::exception &e)
            {
                return;
            }

            if (val < 0)
            {
                return;
            }

            switch (arg_cmd)
            {
            case DEPTH:
                if (val < 0)
                {
                    return;
                }
                else
                {
                    depth = val;
                }
                break;
            case WTIME:
                if (val <= 0)
                {
                    return;
                }
                else
                {
                    wtime = val;
                }
                break;
            case BTIME:
                if (val <= 0)
                {
                    return;
                }
                else
                {
                    btime = val;
                }
                break;
            case MOVES_TO_GO:
                if (val <= 0)
                {
                    return;
                }
                else
                {
                    moves_to_go = val;
                }
                break;
            }
        }
    }

    MovePicker::SearchResult result;
    if (!depth && !infinite && wtime && btime)
    {
        _move_picker.set_max_depth(DEFAULT_MAX_DEPTH);
    }
    else
    {
        _move_picker.set_max_depth(DEFAULT_MIN_DEPTH);
    }

    std::promise<void> signal_exit;
    std::future<void> signal_exit_future = signal_exit.get_future();
    std::thread search_thread(search, std::move(signal_exit_future), std::ref(_move_picker), std::ref(result));
    if ((_board.get_side_to_move() == WHITE && wtime) || (_board.get_side_to_move() == BLACK && btime))
    {
        std::this_thread::sleep_for(std::chrono::milliseconds(timectl::get_time_budget_ms(wtime, btime, _board)));
        signal_exit.set_value();
    }
    search_thread.join();

    std::cout << "bestmove " << result.pv[0].get_uci() << std::endl;
}
