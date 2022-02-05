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
