#include <interfaces/uci/commands/go.hpp>

#include <interfaces/uci/timectl.hpp>

#include <iostream>

#define DEFAULT_DEPTH 32
#define MIN_EVAL (INT_MIN + 1)
#define WINDOW_EXPANSION 50

void interfaces::uci::commands::GoCommand::execute(std::vector<std::string> &args, Board &board)
{
    enum ArgCommand : int
    {
        DEPTH,
        WTIME,
        BTIME,
        MOVES_TO_GO,
    };

    ArgCommand arg_cmd;

    [[maybe_unused]] int moves_to_go{};
    int wtime{}, btime{};
    int depth = DEFAULT_DEPTH;
    for (const std::string &token : args)
    {
        std::cout << "hello world" << std::endl;
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

    MovePicker ai = MovePicker(board);
    ai.setMaxDepth(depth);

    MovePicker::SearchResult result;

    std::promise<void> signal_exit;
    std::future<void> future = signal_exit.get_future();
    std::thread t(this->search, std::move(future), std::move(wtime), std::move(btime), std::ref(ai), std::ref(result));
    std::this_thread::sleep_for(std::chrono::milliseconds(timectl::get_time_budget_ms(wtime, btime, board)));
    signal_exit.set_value();
    t.join();

    std::cout << "bestmove " << Move(result.pv[0]).getUCI() << std::endl;
}

void interfaces::uci::commands::GoCommand::search(std::future<void> future, int wtime, int btime, MovePicker &ai, MovePicker::SearchResult &result)
{
    ai.clearTables();

    int alpha = MIN_EVAL;
    int beta = -MIN_EVAL;
    for (int depth = 1; depth <= ai.getMaxDepth(); depth++)
    {
        auto start = std::chrono::system_clock::now();
        result = ai.findBestMove(depth, alpha, beta);
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

        if (future.wait_for(std::chrono::milliseconds(timectl::get_timeout_ms(depth, wtime, btime, ai.getBoard()))) != std::future_status::timeout)
        {
            break;
        }
    }
}

void interfaces::uci::commands::GoCommand::displaySearchIteration(MovePicker::SearchResult result, int depth, std::chrono::duration<double> elapsed)
{
    std::cout << "info score cp " << result.score
              << " depth " << depth
              << " nodes " << result.nodes
              << " time " << (int)(elapsed / std::chrono::milliseconds(1))
              << " pv ";
    for (const Move &move : result.pv)
    {
        std::cout << move.getUCI() << " ";
    }
    std::cout << std::endl;
}