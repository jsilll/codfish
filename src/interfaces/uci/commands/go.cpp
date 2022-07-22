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

static void displaySearchIteration(MovePicker::SearchResult result, int depth, std::chrono::duration<double> elapsed)
{
    std::cout << "info score cp " << result.score
              << " depth " << depth
              << " nodes " << result.nodes
              << " time " << (int)(elapsed / std::chrono::milliseconds(1))
              << " pv ";

    std::for_each(result.pv.cbegin(), result.pv.cend(), [](const Move &move)
                  { std::cout << move.get_uci() << " "; });
    std::cout << std::endl;
}

static void search(std::future<void> future, MovePicker &ai, MovePicker::SearchResult &result)
{
    ai.clear_tables();

    int alpha = MIN_EVAL;
    int beta = -MIN_EVAL;
    for (int depth = 1; depth <= ai.get_max_depth(); depth++)
    {
        auto start = std::chrono::system_clock::now();
        result = ai.find_best_move(depth, alpha, beta);
        auto end = std::chrono::system_clock::now();

        if ((result.score <= alpha) || (result.score >= beta))
        {
            alpha = MIN_EVAL;
            beta = -MIN_EVAL;
            depth--;
            continue;
        }

        alpha = result.score - WINDOW_EXPANSION;
        beta = result.score + WINDOW_EXPANSION;

        displaySearchIteration(result, depth, end - start);

        if (future.wait_for(std::chrono::milliseconds(1)) == std::future_status::ready)
        {
            break;
        }
    }
}

void uci::GoCommand::execute(std::vector<std::string> &args, Board &board)
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
    MovePicker ai = MovePicker(board);

    if (!depth && wtime && btime)
    {
        ai.set_max_depth(DEFAULT_MAX_DEPTH);
    }
    else
    {
        ai.set_max_depth(DEFAULT_MIN_DEPTH);
    }

    std::promise<void> signal_exit;
    std::future<void> signal_exit_future = signal_exit.get_future();
    std::thread search_thread(search, std::move(signal_exit_future), std::ref(ai), std::ref(result));
    if ((board.get_side_to_move() == WHITE && wtime) || (board.get_side_to_move() == BLACK && btime))
    {
        std::this_thread::sleep_for(std::chrono::milliseconds(timectl::get_time_budget_ms(wtime, btime, board)));
        signal_exit.set_value();
    }
    search_thread.join();

    std::cout << "bestmove " << Move(result.pv[0]).get_uci() << std::endl;
}
