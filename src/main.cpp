#include <iostream>
#include "defs.hpp"
#include "cli.hpp"
#include "tables.hpp"
#include "utils.hpp"
#include "movegen.hpp"

int main(int argc, char const *argv[])
{
    tables::init();

    // std::cout << "Chess Engine Initialized" << std::endl;
    // cli::readCommands();

    U64 occupancy = 0x200c20000048;

    utils::printBB(movegen::getBishopAttacks(D4, occupancy));
    utils::printBB(movegen::getRookAttacks(D4, occupancy));

    return 0;
}