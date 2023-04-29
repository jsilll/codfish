#include <interfaces/cli/commands/commands.hpp>

#include <codchess/movegen/movegen.hpp>

#include <iostream>
#include <algorithm>

void cli::PLMovesCommand::execute([[maybe_unused]] std::vector<std::string> &args)
{
    auto moves = movegen::generate_pseudo_legal_moves(_board);
    std::for_each(moves.begin(), moves.end(), [](const Move &move)
                  { std::cout << move.get_uci() << "\n"; });
    std::cout << "Total number of pseudo legal moves: " << moves.size() << std::endl;
}