#pragma once

#include "../defs.hpp"

#include <string>
#include <vector>

namespace utils
{
  // Common square operations
  inline int getSquare(const int rk, const int fl) { return 8 * rk + fl; }
  inline int getFile(const int sq) { return sq & 7; }
  inline int getRank(const int sq) { return sq >> 3; }
  inline int flipSquare(const int sq) { return sq ^ 56; }

  // Flipping Side to Move
  inline int getOpponent(int to_move) { return to_move ^ 1; }

} // namespace utils