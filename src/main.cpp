#include <iostream>
#include "tables.hpp"
#include "magics.hpp"
#include "cli.hpp"

int main(int argc, char const *argv[])
{
    tables::init();
    magics::init();

    // std::cout << "Chess Engine Initialized" << std::endl;
    // cli::readCommands();

    return 0;
}