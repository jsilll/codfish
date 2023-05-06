#include "commands.hpp"

#include <iostream>

void cli::AsciiCommand::execute([[maybe_unused]] std::vector<std::string> &args)
{
    std::cout << "TODO\n";
    // std::cout << "ascii mode toggled " << (_board.toggle_ascii() ? "on" : "off") << std::endl;
}