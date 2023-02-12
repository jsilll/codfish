#include <codlib/bitboard.hpp>

#include <codlib/utils.hpp>
#include <codlib/movegen/tables.hpp>

#include <string>
#include <iostream>

namespace bitboard
{
    u64 set_occupancy(int index, int bits_in_mask, u64 attack_mask) noexcept 
    {
        u64 occupancy = kZERO;
        for (int bit = 0; bit < bits_in_mask; bit++)
        {
            Square lsb_sq = bit_scan_forward(attack_mask);
            pop_bit(attack_mask, lsb_sq);
            if (index & (1 << bit))
            {
                occupancy |= tables::square_to_bitboard((Square)lsb_sq);
            }
        }

        return occupancy;
    }

    void print(u64 bb) noexcept
    {
        // TODO: implement std::cout << bitboard
        for (int i = RANK_8; i >= RANK_1; i--)
        {
            std::cout << i + 1 << "  ";
            for (int n = FILE_A; n < N_FILES; n++)
            {
                std::cout << ((bb >> utils::get_square((Rank)i, (File)n)) & kONE) << " ";
            }
            std::cout << "\n";
        }
        std::cout << "\n   a b c d e f g h\n\n";
        printf("bitboard %lud\n", bb);
    }
    

} // namespace bitboard