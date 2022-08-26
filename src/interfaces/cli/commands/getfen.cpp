#include <interfaces/cli/commands/commands.hpp>

#include <iostream>

void cli::GetFenCommand::execute([[maybe_unused]] std::vector<std::string> &args)
{
    std::cout << _board.get_fen();
}