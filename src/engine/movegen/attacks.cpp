#include <engine/movegen/attacks.hpp>

#include <engine/utils.hpp>
#include <engine/bitboard.hpp>
#include <engine/movegen/tables.hpp>

namespace attacks
{
  static inline U64 mask_white_pawn_east_attacks(U64 wpawns) { return bitboard::no_ea_one(wpawns); }
  static inline U64 mask_white_pawn_west_attacks(U64 wpawns) { return bitboard::no_we_one(wpawns); }
  static inline U64 mask_black_pawn_east_attacks(U64 bpawns) { return bitboard::so_ea_one(bpawns); }
  static inline U64 mask_black_pawn_west_attacks(U64 bpawns) { return bitboard::so_we_one(bpawns); }

  U64 mask_white_pawn_single_pushes(U64 wpawns, U64 empty) { return bitboard::nort_one(wpawns) & empty; }
  U64 mask_black_pawn_single_pushes(U64 bpawns, U64 empty) { return bitboard::sout_one(bpawns) & empty; }

  U64 mask_white_pawn_double_pushes(U64 wpawns, U64 empty)
  {
    U64 single_pushes = mask_white_pawn_single_pushes(wpawns, empty);
    return bitboard::nort_one(single_pushes) & empty & tables::MASK_RANK[3];
  }

  U64 mask_black_pawn_double_pushes(U64 bpawns, U64 empty)
  {
    U64 single_pushes = mask_black_pawn_single_pushes(bpawns, empty);
    return bitboard::sout_one(single_pushes) & empty & tables::MASK_RANK[4];
  }

  U64 mask_white_pawn_any_attacks(U64 wpawns) { return mask_white_pawn_east_attacks(wpawns) | mask_white_pawn_west_attacks(wpawns); }
  U64 mask_black_pawn_any_attacks(U64 bpawns) { return mask_black_pawn_east_attacks(bpawns) | mask_black_pawn_west_attacks(bpawns); }

  U64 mask_knight_attacks(U64 knights)
  {
    static const U64 CLEAR_FILE_HG = 0x3f3f3f3f3f3f3f3f;
    static const U64 CLEAR_FILE_AB = 0xfcfcfcfcfcfcfcfc;

    U64 l1 = (knights >> 1) & tables::MASK_CLEAR_FILE[7];
    U64 r1 = (knights << 1) & tables::MASK_CLEAR_FILE[0];
    U64 h1 = l1 | r1;

    U64 l2 = (knights >> 2) & CLEAR_FILE_HG;
    U64 r2 = (knights << 2) & CLEAR_FILE_AB;
    U64 h2 = l2 | r2;

    return (h1 << 16) | (h1 >> 16) | (h2 << 8) | (h2 >> 8);
  }

  U64 mask_king_attacks(U64 kings)
  {
    U64 attacks = bitboard::east_one(kings) | bitboard::west_one(kings);
    kings |= attacks;
    attacks |= bitboard::nort_one(kings) | bitboard::sout_one(kings);
    return attacks;
  }

  U64 mask_bishop_attack_rays(int sq)
  {
    U64 attacks = ZERO;
    int rank = utils::get_rank(sq);
    int file = utils::get_file(sq);

    for (int r = rank + 1, f = file + 1; r < 7 && f < 7; r++, f++)
    {
      attacks |= (ONE << utils::get_square(r, f));
    }

    for (int r = rank + 1, f = file - 1; r < 7 && f > 0; r++, f--)
    {
      attacks |= (ONE << utils::get_square(r, f));
    }

    for (int r = rank - 1, f = file + 1; r > 0 && f < 7; r--, f++)
    {
      attacks |= (ONE << utils::get_square(r, f));
    }

    for (int r = rank - 1, f = file - 1; r > 0 && f > 0; r--, f--)
    {
      attacks |= (ONE << utils::get_square(r, f));
    }

    return attacks;
  }

  U64 mask_rook_attack_rays(int sq)
  {
    U64 attacks = ZERO;
    int rank = utils::get_rank(sq);
    int file = utils::get_file(sq);

    for (int r = rank + 1; r < 7; r++)
    {
      attacks |= (ONE << utils::get_square(r, file));
    }

    for (int r = rank - 1; r > 0; r--)
    {
      attacks |= (ONE << utils::get_square(r, file));
    }

    for (int f = file + 1; f < 7; f++)
    {
      attacks |= (ONE << utils::get_square(rank, f));
    }

    for (int f = file - 1; f > 0; f--)
    {
      attacks |= (ONE << utils::get_square(rank, f));
    }

    return attacks;
  }

  U64 mask_bishop_attacks(int sq, U64 block)
  {
    U64 attacks = ZERO;
    int rank = utils::get_rank(sq);
    int file = utils::get_file(sq);

    for (int r = rank + 1, f = file + 1; r < 8 && f < 8; r++, f++)
    {
      attacks |= (ONE << utils::get_square(r, f));
      if ((ONE << utils::get_square(r, f)) & block)
      {
        break;
      }
    }

    for (int r = rank + 1, f = file - 1; r < 8 && f >= 0; r++, f--)
    {
      attacks |= (ONE << utils::get_square(r, f));
      if ((ONE << utils::get_square(r, f)) & block)
      {
        break;
      }
    }

    for (int r = rank - 1, f = file + 1; r >= 0 && f < 8; r--, f++)
    {
      attacks |= (ONE << utils::get_square(r, f));
      if ((ONE << utils::get_square(r, f)) & block)
      {
        break;
      }
    }

    for (int r = rank - 1, f = file - 1; r >= 0 && f >= 0; r--, f--)
    {
      attacks |= (ONE << utils::get_square(r, f));
      if ((ONE << utils::get_square(r, f)) & block)
      {
        break;
      }
    }

    return attacks;
  }

  U64 mask_rook_attacks(int sq, U64 block)
  {
    U64 attacks = ZERO;
    int rank = utils::get_rank(sq);
    int file = utils::get_file(sq);

    for (int r = rank + 1; r < 8; r++)
    {
      attacks |= (ONE << utils::get_square(r, file));
      if ((ONE << utils::get_square(r, file)) & block)
      {
        break;
      }
    }

    for (int r = rank - 1; r >= 0; r--)
    {
      attacks |= (ONE << utils::get_square(r, file));
      if ((ONE << utils::get_square(r, file)) & block)
      {
        break;
      }
    }

    for (int f = file + 1; f < 8; f++)
    {
      attacks |= (ONE << utils::get_square(rank, f));
      if ((ONE << utils::get_square(rank, f)) & block)
      {
        break;
      }
    }

    for (int f = file - 1; f >= 0; f--)
    {
      attacks |= (ONE << utils::get_square(rank, f));
      if ((ONE << utils::get_square(rank, f)) & block)
      {
        break;
      }
    }

    return attacks;
  }

} // namespace attacks