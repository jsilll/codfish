#pragma once

#include <codlib/constants.hpp>

#include <string>
#include <vector>

namespace utils
{
  /// @brief Returns the file of a square
  /// @param sq The square
  /// @return The file
  [[nodiscard]] constexpr File get_file(const Square sq) noexcept { return static_cast<File>(sq & 7); }

  /// @brief Returns the rank of a square
  /// @param sq The square
  /// @return The rank
  [[nodiscard]] constexpr Rank get_rank(const Square sq) noexcept { return static_cast<Rank>(sq >> 3); }

  /// @brief Returns the flipped square
  /// @param sq The square
  /// @return The flipped square
  [[nodiscard]] constexpr Square flip_square(const Square sq) noexcept { return static_cast<Square>(sq ^ 56); }

  /// @brief Returns the opponent color
  /// @param to_move The color to move
  /// @return The opponent color
  [[nodiscard]] constexpr Color get_opponent(Color to_move) noexcept { return static_cast<Color>((int)to_move ^ 1); }

  /// @brief Returns the square of a rank and file
  /// @param rk The rank
  /// @param fl The file
  /// @return The square
  [[nodiscard]] constexpr Square get_square(const Rank rk, const File fl) noexcept { return static_cast<Square>(8 * rk + fl); }
} // namespace utils