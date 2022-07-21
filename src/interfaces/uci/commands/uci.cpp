#include <interfaces/uci/commands/uci.hpp>

#include <iostream>

void interfaces::uci::commands::UCICommand::execute([[maybe_unused]] std::vector<std::string> &args, [[maybe_unused]] Board &board)
{
    std::cout << "id name Chess Engine\n";
    std::cout << "id name Chess Engine\n";
    std::cout << "uciok" << std::endl;
}