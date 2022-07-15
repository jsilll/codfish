#include <interfaces/uci/commands/isready.hpp>

#include <iostream>

void interfaces::uci::commands::IsReadyCommand::execute([[maybe_unused]] std::vector<std::string> &args, [[maybe_unused]] Board &board)
{
    std::cout << "readyok" << std::endl;
}