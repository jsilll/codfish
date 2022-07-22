#include <interfaces/cli/commands/commands.hpp>

#include <iostream>

void cli::GetFenCommand::execute([[maybe_unused]] std::vector<std::string> &args, Board &board)
{
    std::cout << board.get_fen();
}