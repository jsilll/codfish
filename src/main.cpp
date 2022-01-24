#include <iostream>
#include "commands.hpp"

int main(int argc, char const *argv[])
{
    // tables::init();

    // for (int sq = 0; sq < 64; sq++)
    // {
    //     utils::print_bb(tables::KING_ATTACKS[sq]);
    // }

    // std::string line;
    // std::getline(std::cin, line);
    // std::cout << line << std::endl;

    std::cout << "Chess Engine Beta" << std::endl;
    cli::readCommands();

    return 0;
}