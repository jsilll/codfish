#include <iostream>
#include "cli.hpp"
#include "tables.hpp"

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

    tables::init();
    std::cout << "Chess Engine (Pre-Release) Initialized" << std::endl;
    cli::readCommands();

    return 0;
}