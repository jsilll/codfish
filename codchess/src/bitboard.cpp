#include <codchess/bitboard.hpp>

#include <iostream>
#include <string>

#include <codchess/utils.hpp>

namespace cod::chess::bitboard {
Bitboard
SetOccupancy(const int index, const int bits_in_mask,
             Bitboard attack_mask) noexcept {
    auto occupancy{kZero};
    for (int bit = 0; bit < bits_in_mask; ++bit) {
        const auto lsb_sq = BitScanForward(attack_mask);
        PopBit(attack_mask, lsb_sq);
        if (index & (1 << bit)) {
            occupancy |= utils::kSquareToBitboard[static_cast<std::size_t>(lsb_sq)];
        }
    }
    return occupancy;
}

Square
BitScanForward(const Bitboard bb) noexcept {
    static constexpr int index64[64] = {
        0,  47, 1,  56, 48, 27, 2,  60, 57, 49, 41, 37, 28, 16, 3,  61,
        54, 58, 35, 52, 50, 42, 21, 44, 38, 32, 29, 23, 17, 11, 4,  62,
        46, 55, 26, 59, 40, 36, 15, 53, 34, 51, 20, 43, 31, 22, 10, 45,
        25, 39, 14, 33, 19, 30, 9,  24, 13, 18, 8,  12, 7,  6,  5,  63};
    constexpr auto debruijn64 = 0x03f79d71b4cb0a89;
    return static_cast<Square>(index64[((bb ^ (bb - 1)) * debruijn64) >> 58]);
}

[[maybe_unused]] void
Display(std::ostream &os, const Bitboard &bb) noexcept {
    for (int i = static_cast<int>(Rank::R8);
         i >= static_cast<int>(Rank::R1); --i) {
        os << (i + 1) << "  ";
        for (int n = static_cast<int>(File::FA);
             n < static_cast<int>(File::Total); ++n) {
            os << static_cast<int>(
                      (bb >> static_cast<int>(utils::GetSquare(
                                 static_cast<Rank>(i), static_cast<File>(n)))) &
                      kOne)
               << " ";
        }
        os << "\n";
    }
    os << "\n   a b c d e f g h\n";
}
}   // namespace cod::chess::bitboard