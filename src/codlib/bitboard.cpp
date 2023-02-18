#include <codlib/bitboard.hpp>

#include <iostream>
#include <string>

#include <codlib/utils.hpp>

namespace bitboard {
    Bitboard set_occupancy(const int index, const int bits_in_mask, Bitboard attack_mask) noexcept {
        Bitboard occupancy = ZERO;
        for (int bit = 0; bit < bits_in_mask; bit++) {
            Square lsb_sq = bit_scan_forward(attack_mask);
            pop_bit(attack_mask, lsb_sq);
            if (index & (1 << bit)) { occupancy |= utils::SQUARE_BB[lsb_sq]; }
        }

        return occupancy;
    }

    void Display(std::ostream &os, const Bitboard &bb) noexcept {
        for (int i = RANK_8; i >= RANK_1; --i) {
            os << (i + 1) << std::string("  ");
            for (int n = FILE_A; n < N_FILES; ++n) {
                os << static_cast<int>((bb >> utils::get_square((Rank) i, (File) n)) & ONE) << std::string(" ");
            }
            os << "\n";
        }
        os << "\n   a b c d e f g h\n";
    }
}// namespace bitboard