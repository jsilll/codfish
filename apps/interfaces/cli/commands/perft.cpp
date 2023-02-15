#include <interfaces/cli/commands/commands.hpp>

#include "codlib/perft.hpp"

#include <chrono>
#include <iostream>

static void timed_perft(int depth, Board &board)
{
    std::chrono::time_point<std::chrono::system_clock> start = std::chrono::system_clock::now();
    int nodes = perft::perft(board, depth);
    std::chrono::time_point<std::chrono::system_clock> end = std::chrono::system_clock::now();
    std::chrono::duration<double> elapsed_seconds = end - start;
    std::time_t end_time = std::chrono::system_clock::to_time_t(end);
    std::cout << "Found " << nodes << " nodes." << std::endl;
    std::cout << "Finished computation at " << std::ctime(&end_time);
    std::cout << "Elapsed time: " << elapsed_seconds.count() << "s" << std::endl;
    std::cout << "Nodes Per Second: " << (double)nodes / elapsed_seconds.count() << std::endl;
}

void cli::PerftCommand::execute(std::vector<std::string> &args)
{

    if (args.size() == 0)
    {
        std::cout << "perft command takes exactly one argument" << std::endl;
        return;
    }

    int depth = 0;
    try
    {
        depth = std::stoi(args[0]);
    }
    catch (const std::exception &e)
    {
        std::cout << "invalid depth value." << std::endl;
        return;
    }
    if (depth >= 0)
    {
        timed_perft(depth, _board);
    }
    else
    {
        std::cout << "invalid depth value." << std::endl;
    }
}
