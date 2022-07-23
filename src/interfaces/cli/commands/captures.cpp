#include <interfaces/cli/commands/commands.hpp>

#include <engine/movegen/movegen.hpp>

#include <iostream>
#include <algorithm>

void cli::CapturesCommand::execute([[maybe_unused]] std::vector<std::string> &args, Board &board)
{
    std::vector<Move> captures = movegen::generate_legal_captures(board);
    std::for_each(captures.begin(), captures.end(), [](const Move &capture)
                  { std::cout << capture.get_uci() << "\n"; });
    std::cout << "Total number of captures: " << captures.size() << std::endl;
}