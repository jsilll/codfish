#include <iostream>
#include "defs.hpp"
#include "utils.hpp"
#include "magics.hpp"
#include "tables.hpp"
#include "movegen.hpp"
#include "board.hpp"
#include "cli.hpp"

int main(int argc, char const *argv[])
{
    magics::init();
    tables::init();

    std::cout << "Chess Engine Initialized" << std::endl;
    cli::readCommands();

    return 0;
}