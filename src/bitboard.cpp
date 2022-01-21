#include "bitboard.hpp"

#include <iostream>

void bb::print_bb(U64 bitboard)
{
    for (int i = 7; i >= 0; i--)
    {
        std::cout << i + 1 << "  ";
        for (int n = 0; n < 8; n++)
        {
            std::cout << ((bitboard >> get_square_index(i, n)) & ONE) << " ";
        }
        std::cout << "\n";
    }
    std::cout << "\n   a b c d e f g h\n\n";
    printf("   Bitboard %llud\n", bitboard);
    std::cout << std::endl;
}