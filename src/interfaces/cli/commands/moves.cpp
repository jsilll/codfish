#include <interfaces/cli/commands/commands.hpp>

#include <engine/movegen/movegen.hpp>

#include <iostream>
#include <algorithm>

void cli::MovesCommand::execute([[maybe_unused]] std::vector<std::string> &args)
{
    auto moves = movegen::generateLegalMoves(_board);
    std::for_each(moves.begin(), moves.end(), [](const Move &move)
                  { std::cout << move.get_uci() << "\n"; });
    std::cout << "Total number of moves: " << moves.size() << std::endl;
}