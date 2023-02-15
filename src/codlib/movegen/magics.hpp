#pragma once

#include "../../../include/codlib/base.hpp"
#include "codlib/bitboard.hpp"

namespace magics {
/// @brief A struct that contains the magic number, the mask and the shift for a given square.
struct Magic
{
  /// @brief The mask for a given square.
  bitboard::u64 mask;
  /// @brief The magic number for a given square.
  bitboard::u64 magic;
  /// @brief The shift for a given square.
  int shift;
};

/// @brief Initializes the magic tables.
void init() noexcept;

/// @brief Array of pre-computed magic numbers, masks and shifts for rooks.
extern Magic MAGIC_TABLE_ROOK[N_SQUARES];

/// @brief Array of pre-computed magic numbers, masks and shifts for bishops.
extern Magic MAGIC_TABLE_BISHOP[N_SQUARES];

/// @brief Computes the magic index for a given occupancy and magic entry
/// @param occ The occupancy
/// @param magic The magic entry
/// @return The magic index
[[nodiscard]] constexpr bitboard::u64 magic_index(bitboard::u64 occ, const magics::Magic &magic) noexcept
{
  occ &= magic.mask;
  occ *= magic.magic;
  occ >>= magic.shift;
  return occ;
}
}// namespace magics