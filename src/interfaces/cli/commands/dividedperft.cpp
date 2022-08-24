#include <interfaces/cli/commands/commands.hpp>

#include <engine/bitboard.hpp>

#include <engine/movegen/movegen.hpp>
#include <engine/movegen/perft.hpp>

#include <chrono>
#include <iostream>

static void dperft(int depth, Board &board)
{
    int total_nodes = 0;
    std::chrono::time_point<std::chrono::system_clock> start = std::chrono::system_clock::now();
    for (const Move &move : movegen::generate_pseudo_legal_moves(board))
    {
        Board::GameState state = board.get_state();
        board.make(move);
        Square king_sq = bitboard::bit_scan_forward(board.get_pieces(board.get_opponent(), KING));
        Color attacker_side = board.get_side_to_move();
        if (!board.is_square_attacked(king_sq, attacker_side))
        {
            int nodes = perft::perft(board, depth - 1);
            std::cout << move.get_uci() << ": " << nodes << std::endl;
            total_nodes += nodes;
        }
        board.unmake(move, state);
    }
    std::chrono::time_point<std::chrono::system_clock> end = std::chrono::system_clock::now();
    std::chrono::duration<double> elapsed_seconds = end - start;
    std::time_t end_time = std::chrono::system_clock::to_time_t(end);
    std::cout << "Found " << total_nodes << " nodes." << std::endl;
    std::cout << "Finished computation at " << std::ctime(&end_time);
    std::cout << "Elapsed time: " << elapsed_seconds.count() << "s" << std::endl;
    std::cout << "Nodes Per Second: " << (double)total_nodes / elapsed_seconds.count() << std::endl;
}

void cli::DividedPerftCommand::execute(std::vector<std::string> &args)
{
    if (args.size() == 0)
    {
        std::cout << "dperft command takes exactly one argument" << std::endl;
        return;
    }

    int depth = 0;
    try
    {
        depth = std::stoi(args[0]);
    }
    catch (const std::exception &e)
    {
        std::cout << "invalid depth value1." << std::endl;
        return;
    }

    if (depth >= 0)
    {
        dperft(depth, _board);
    }
    else
    {
        std::cout << "invalid depth value." << std::endl;
    }
}