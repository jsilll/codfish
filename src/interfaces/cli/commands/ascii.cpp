#include <interfaces/cli/commands/commands.hpp>

#include <iostream>

void cli::AsciiCommand::execute([[maybe_unused]] std::vector<std::string> &args, Board &board)
{
    std::cout << "ascii mode toggled " << (board.toggle_ascii() ? "on" : "off") << std::endl;
}