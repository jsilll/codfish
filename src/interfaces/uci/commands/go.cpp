#include <interfaces/uci/commands/go.hpp>

#include <iostream>

#define WINDOW_EXPANSION 50
#define MIN_EVAL (INT_MIN + 1)
#define DEFAULT_DEPTH 9

void interfaces::uci::commands::GoCommand::execute(std::vector<std::string> &args, Board &board)
{
    int max_depth = DEFAULT_DEPTH;

    bool in_depth_command = false;
    for (const std::string &token : args)
    {
        if (token == "depth")
        {
            in_depth_command = true;
        }

        if (in_depth_command)
        {
            try
            {
                max_depth = std::stoi(token);
            }
            catch (const std::exception &e)
            {
                return;
            }

            if (max_depth < 0)
            {
                return;
            }

            in_depth_command = false;
        }
    }

    MovePicker ai = MovePicker(board);
    ai.setMaxDepth(max_depth);
    MovePicker::SearchResult result;

    std::promise<void> signal_exit;
    std::future<void> future = signal_exit.get_future();
    std::thread t(this->search, std::ref(ai), std::ref(result), std::move(future));
    std::cout << "going to sleep" << std::endl;
    std::this_thread::sleep_for(std::chrono::seconds(20));
    signal_exit.set_value();
    t.join();

    std::cout << "bestmove " << Move(result.pv[0]).getUCI() << std::endl;
}

void interfaces::uci::commands::GoCommand::search(MovePicker &ai, MovePicker::SearchResult &result, std::future<void> future)
{
    int alpha = MIN_EVAL;
    int beta = -MIN_EVAL;

    ai.clearTables();
    for (int depth = 1; depth <= ai.getMaxDepth(); depth++)
    {
        if (future.wait_for(std::chrono::milliseconds(1)) != std::future_status::timeout)
        {
            break;
        }

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