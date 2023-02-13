#pragma once

#include <codlib/bitboard.hpp>
#include <codlib/constants.hpp>

namespace magics
{
  /// @brief A struct that contains the magic number, the mask and the shift for a given square.
  struct Magic
  {
    /// @brief The mask for a given square.
    u64 mask;
    /// @brief The magic number for a given square.
    u64 magic;
    /// @brief The shift for a given square.
    int shift;
  };

  /// @brief Initializes the magic tables.
  void init() noexcept;

  /// @brief Array of pre-computed magic numbers, masks and shifts for rooks.
  extern Magic MAGIC_TABLE_ROOK[N_SQUARES];

  /// @brief Array of pre-computed magic numbers, masks and shifts for bishops.
  extern Magic MAGIC_TABLE_BISHOP[N_SQUARES];

  /// @brief Generates new magic numbers and prints them to stdout.
  void generate() noexcept;
} // namespace magics