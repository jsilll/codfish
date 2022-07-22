#pragma once

#include <string>
#include <vector>

namespace utils
{
  // Square Operations
  inline int get_square(const int rk, const int fl) { return 8 * rk + fl; }
  inline int get_file(const int sq) { return sq & 7; }
  inline int get_rank(const int sq) { return sq >> 3; }
  inline int flipSquare(const int sq) { return sq ^ 56; }

  // Flip Side to Move
  inline int get_opponent(int to_move) { return to_move ^ 1; }

} // namespace utils