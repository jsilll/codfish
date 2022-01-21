#include "utils.hpp"

int main(int argc, char const *argv[])
{
    tables::init();

    for (int sq = 0; sq < 64; sq++)
    {
        utils::print_bb(tables::PAWN_ATTACKS[bb::WHITE][sq]);
    }

    return 0;
}