#include "commands.hpp"

#include <iostream>
#include <algorithm>

void cli::CapturesCommand::execute([[maybe_unused]] std::vector<std::string> &args)
{
    std::cout << "TODO\n";
    // std::vector<Move> captures = movegen::generate_legal_captures(_board);
    // std::for_each(captures.begin(), captures.end(), [](const Move &capture)
    //               { std::cout << capture.get_uci() << "\n"; });
    // std::cout << "Total number of captures: " << captures.size() << std::endl;
}