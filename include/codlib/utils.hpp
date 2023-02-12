#pragma once

#include <codlib/constants.hpp>

#include <string>
#include <vector>

namespace utils
{
  
  [[nodiscard]] constexpr File get_file(const Square sq) noexcept { return (File)(sq & 7); }

  [[nodiscard]] constexpr Rank get_rank(const Square sq) noexcept { return (Rank)(sq >> 3); }

  [[nodiscard]] constexpr Square flip_square(const Square sq) noexcept { return (Square)(sq ^ 56); }

  [[nodiscard]] constexpr Square get_square(const Rank rk, const File fl) noexcept { return (Square)(8 * rk + fl); }

  [[nodiscard]] constexpr Color get_opponent(Color to_move) noexcept { return (Color)((int)to_move ^ 1); }

} // namespace utils