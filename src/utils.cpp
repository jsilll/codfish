#include "utils.hpp"

#include "tables.hpp"
#include <iostream>

int utils::bitScan(U64 bitboard)
{
    if (bitboard)
    {
        return utils::bitCount((bitboard & -bitboard) - 1);
    }
    else
    {
        return -1;
    }
}

U64 utils::setOccupancy(int sq, int bits_in_mask, U64 attack_mask)
{
    U64 occupancy = ZERO;
    for (int i = 0; i < bits_in_mask; i++)
    {
        int lsb_sq = utils::bitScan(attack_mask);
        utils::popBit(attack_mask, lsb_sq);
        if (sq & (1 << i))
        {
            occupancy |= tables::SQUARE_BB[lsb_sq];
        }
    }
    return occupancy;
}

void utils::printBB(U64 bitboard)
{
    for (int i = 7; i >= 0; i--)
    {
        std::cout << i + 1 << "  ";
        for (int n = 0; n < 8; n++)
        {
            std::cout << ((bitboard >> getSquare(i, n)) & ONE) << " ";
        }
        std::cout << "\n";
    }
    std::cout << "\n   a b c d e f g h\n\n";
    printf("bitboard %llud\n", bitboard);
}
