#include <iostream>
#include "defs.hpp"
#include "cli.hpp"
#include "tables.hpp"
#include "utils.hpp"
#include "movegen.hpp"

int main(int argc, char const *argv[])
{
    tables::init();

    std::cout << "Chess Engine Initialized" << std::endl;
    cli::readCommands();

    return 0;
}