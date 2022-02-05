#include "utils.hpp"

#include "tables.hpp"
#include <iostream>

unsigned int utils::bitCntHakmem(U64 bitmap)
{
    // MIT HAKMEM algorithm, see http://graphics.stanford.edu/~seander/bithacks.html
    static const U64 M1 = 0x5555555555555555;  // 1 zero,  1 one ...
    static const U64 M2 = 0x3333333333333333;  // 2 zeros,  2 ones ...
    static const U64 M4 = 0x0f0f0f0f0f0f0f0f;  // 4 zeros,  4 ones ...
    static const U64 M8 = 0x00ff00ff00ff00ff;  // 8 zeros,  8 ones ...
    static const U64 M16 = 0x0000ffff0000ffff; // 16 zeros, 16 ones ...
    static const U64 M32 = 0x00000000ffffffff; // 32 zeros, 32 ones

    bitmap = (bitmap & M1) + ((bitmap >> 1) & M1);    //put count of each  2 bits into those  2 bits
    bitmap = (bitmap & M2) + ((bitmap >> 2) & M2);    //put count of each  4 bits into those  4 bits
    bitmap = (bitmap & M4) + ((bitmap >> 4) & M4);    //put count of each  8 bits into those  8 bits
    bitmap = (bitmap & M8) + ((bitmap >> 8) & M8);    //put count of each 16 bits into those 16 bits
    bitmap = (bitmap & M16) + ((bitmap >> 16) & M16); //put count of each 32 bits into those 32 bits
    bitmap = (bitmap & M32) + ((bitmap >> 32) & M32); //put count of each 64 bits into those 64 bits
    return (int)bitmap;
}

int utils::bitScan(U64 bitboard)
{
    if (bitboard)
    {
        return utils::bitCntKernighan((bitboard & -bitboard) - 1);
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
