#include <engine/bitboard.hpp>

#include <engine/utils.hpp>
#include <engine/movegen/tables.hpp>

#include <string>
#include <iostream>

namespace bitboard
{
    int bit_count(u64 bb)
    {
        unsigned int count = 0;
        while (bb)
        {
            count++;
            bb &= bb - 1;
        }
        return (int)count;
    }

    int bit_scan(u64 bb)
    {
        if (bb)
        {
            return bit_count((bb & -bb) - 1);
        }
        return -1;
    }

    int bit_scan_forward(u64 bb)
    {
        static const int index64[64] = {
            0, 47, 1, 56, 48, 27, 2, 60,
            57, 49, 41, 37, 28, 16, 3, 61,
            54, 58, 35, 52, 50, 42, 21, 44,
            38, 32, 29, 23, 17, 11, 4, 62,
            46, 55, 26, 59, 40, 36, 15, 53,
            34, 51, 20, 43, 31, 22, 10, 45,
            25, 39, 14, 33, 19, 30, 9, 24,
            13, 18, 8, 12, 7, 6, 5, 63};

        static const u64 debruijn64 = 0x03f79d71b4cb0a89;
        return index64[((bb ^ (bb - 1)) * debruijn64) >> 58];
    }

    u64 set_occupancy(int index, int bits_in_mask, u64 attack_mask)
    {
        u64 occupancy = ZERO;
        for (int bit = 0; bit < bits_in_mask; bit++)
        {
            int lsb_sq = bit_scan_forward(attack_mask);
            pop_bit(attack_mask, lsb_sq);
            if (index & (1 << bit))
            {
                occupancy |= tables::SQUARE_BB[lsb_sq];
            }
        }
        return occupancy;
    }

    void print(u64 bb)
    {
        for (int i = 7; i >= 0; i--)
        {
            std::cout << i + 1 << "  ";
            for (int n = 0; n < 8; n++)
            {
                std::cout << ((bb >> utils::get_square(i, n)) & ONE) << " ";
            }
            std::cout << "\n";
        }
        std::cout << "\n   a b c d e f g h\n\n";
        printf("bitboard %lud\n", bb);
    }

} // namespace bitboard