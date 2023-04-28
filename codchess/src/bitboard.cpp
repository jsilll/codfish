#include <codchess/bitboard.hpp>

#include <iostream>
#include <string>

#include <codchess/utils.hpp>

namespace codchess::bitboard {
Bitboard
SetOccupancy(const int index, const int bits_in_mask,
             Bitboard attack_mask) noexcept {
    auto occupancy{ZERO};
    for (int bit = 0; bit < bits_in_mask; bit++) {
        const auto lsb_sq = BitScanForward(attack_mask);
        PopBit(attack_mask, lsb_sq);
        if (index & (1 << bit)) {
            occupancy |= utils::SQUARE_BB[lsb_sq];
        }
    }

    return occupancy;
}

void
Display(std::ostream &os, const Bitboard &bb) noexcept {
    for (int i = RANK_8; i >= RANK_1; --i) {
        os << (i + 1) << std::string("  ");
        for (int n = FILE_A; n < N_FILES; ++n) {
            os << static_cast<int>(
                      (bb >> utils::GetSquare((Rank) i, (File) n)) & ONE)
               << std::string(" ");
        }
        os << "\n";
    }
    os << "\n   a b c d e f g h\n";
}
}   // namespace codchess::bitboard