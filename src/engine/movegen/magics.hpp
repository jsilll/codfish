#pragma once

#include <engine/bitboard.hpp>
#include <engine/constants.hpp>

namespace magics
{
  struct Magic
  {
    u64 mask;
    u64 magic;
    int shift;
  };

  void generate();

  void init();

  Magic get_bishop_magic(Square sq);
  Magic get_rook_magic(Square sq);

} // namespace magics