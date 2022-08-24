#pragma once

#include <engine/constants.hpp>

#include <string>
#include <vector>

namespace utils
{
  // Square Operations
  inline Square get_square(const Rank rk, const File fl) { return (Square)(8 * rk + fl); }
  inline Square flip_square(const Square sq) { return (Square)(sq ^ 56); }

  inline File get_file(const Square sq) { return (File)(sq & 7); }
  inline Rank get_rank(const Square sq) { return (Rank)(sq >> 3); }

  // Flip Side to Move
  inline Color get_opponent(Color to_move) { return (Color)((int)to_move ^ 1); }

} // namespace utils