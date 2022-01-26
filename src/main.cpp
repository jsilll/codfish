#include <iostream>
#include "cli.hpp"
#include "tables.hpp"

int main(int argc, char const *argv[])
{
    tables::init();
    std::cout << "Chess Engine Initialized" << std::endl;
    cli::readCommands();
    return 0;
}