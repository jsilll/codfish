#include <interfaces/uci/commands/commands.hpp>

#include <iostream>

void uci::UCICommand::execute([[maybe_unused]] std::vector<std::string> &args)
{
    std::cout << "id name Chess Engine\n";
    std::cout << "id name Chess Engine\n";
    std::cout << "uciok" << std::endl;
}