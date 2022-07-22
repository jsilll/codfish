#include <interfaces/cli/commands/commands.hpp>

#include <iostream>

void cli::RotateCommand::execute([[maybe_unused]] std::vector<std::string> &args, Board &board)
{
    std::cout << (board.rotate_display() ? "white" : "black") << " is now on bottom" << std::endl;
}