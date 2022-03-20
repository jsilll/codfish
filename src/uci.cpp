#include "uci.hpp"

#include "magics.hpp"
#include "tables.hpp"

void Uci::init()
{
    Magics::init();
    Tables::init();
}