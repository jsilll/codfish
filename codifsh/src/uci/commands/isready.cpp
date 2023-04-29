#include <interfaces/uci/commands/commands.hpp>

#include <iostream>

void uci::IsReadyCommand::execute([[maybe_unused]] std::vector<std::string> &args)
{
    std::cout << "readyok" << std::endl;
}