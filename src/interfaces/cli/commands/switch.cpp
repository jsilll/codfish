#include <interfaces/cli/commands/commands.hpp>

#include <iostream>

void cli::SwitchCommand::execute([[maybe_unused]] std::vector<std::string> &args, Board &board)
{
    std::cout << "side to play is now " << (board.switch_side_to_move() == WHITE ? "white" : "black") << std::endl;
}