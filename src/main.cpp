#include <iostream>
#include "defs.hpp"
#include "utils.hpp"
#include "magics.hpp"
#include "tables.hpp"
#include "board.hpp"
#include "cli.hpp"
#include "uci.hpp"

int main(int argc, char const *argv[])
{
    Magics::init();
    Tables::init();

    std::cout << "Chess Engine Initialized" << std::endl;

    Cli::init();
    // UCI::init();
    // Lichess::init(); ??

    return 0;
}