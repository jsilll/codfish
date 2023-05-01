#pragma once

#include <codchess/base.hpp>
#include <codchess/bitboard.hpp>

namespace codchess::magics {
/// @brief A struct that contains the magic number, the mask and the shift for a
/// given square.
struct Magic {
    /// @brief The mask for a given square.
    bitboard::Bitboard mask;
    /// @brief The magic number for a given square.
    bitboard::Bitboard magic;
    /// @brief The shift for a given square.
    std::int32_t shift;
};

/// @brief Initializes the magic tables.
void Init() noexcept;

/// @brief Array of pre-computed magic numbers, masks and shifts for rooks.
extern Magic ROOK_MAGIC_TABLE[N_SQUARES];

/// @brief Array of pre-computed magic numbers, masks and shifts for bishops.
extern Magic BISHOP_MAGIC_TABLE[N_SQUARES];

/// @brief Computes the magic index for a given occupancy and magic entry
/// @param occ The occupancy
/// @param magic The magic entry
/// @return The magic index
[[nodiscard]] constexpr bitboard::Bitboard
MagicIndex(bitboard::Bitboard occ, const Magic &magic) noexcept {
    occ &= magic.mask;
    occ *= magic.magic;
    occ >>= magic.shift;
    return occ;
}
}   // namespace codchess::magics