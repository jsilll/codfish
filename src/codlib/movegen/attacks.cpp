#include <codlib/movegen/attacks.hpp>

#include <codlib/utils.hpp>
#include <codlib/bitboard.hpp>
#include <codlib/movegen/tables.hpp>

namespace attacks
{
  /// @brief Returns a bitboard of all east attacks of white pawns.
  /// @param wpawns A bitboard of white pawns.
  /// @return A bitboard of all east attacks of white pawns.
  [[nodiscard]] constexpr u64 mask_white_pawn_east_attacks(u64 wpawns) noexcept { return bitboard::no_ea_one(wpawns); }

  /// @brief Returns a bitboard of all west attacks of white pawns.
  /// @param wpawns A bitboard of white pawns.
  /// @return A bitboard of all west attacks of white pawns.
  [[nodiscard]] constexpr u64 mask_white_pawn_west_attacks(u64 wpawns) noexcept { return bitboard::no_we_one(wpawns); }

  /// @brief Returns a bitboard of all east attacks of black pawns.
  /// @param bpawns A bitboard of black pawns.
  /// @return A bitboard of all east attacks of black pawns.
  [[nodiscard]] constexpr u64 mask_black_pawn_east_attacks(u64 bpawns) noexcept { return bitboard::so_ea_one(bpawns); }

  /// @brief Returns a bitboard of all west attacks of black pawns.
  /// @param bpawns A bitboard of black pawns.
  /// @return A bitboard of all west attacks of black pawns.
  [[nodiscard]] constexpr u64 mask_black_pawn_west_attacks(u64 bpawns) noexcept { return bitboard::so_we_one(bpawns); }

  /// @brief Returns a bitboard of all single pushes of white pawns.
  /// @param wpawns A bitboard of white pawns.
  /// @param empty A bitboard of empty squares.
  u64 mask_white_pawn_single_pushes(u64 wpawns, u64 empty) noexcept { return bitboard::nort_one(wpawns) & empty; }

  /// @brief Returns a bitboard of all single pushes of black pawns.
  /// @param bpawns A bitboard of black pawns.
  /// @param empty A bitboard of empty squares.
  /// @return
  u64 mask_black_pawn_single_pushes(u64 bpawns, u64 empty) noexcept { return bitboard::sout_one(bpawns) & empty; }

  u64 mask_white_pawn_double_pushes(u64 wpawns, u64 empty) noexcept
  {
    u64 single_pushes = mask_white_pawn_single_pushes(wpawns, empty);
    return bitboard::nort_one(single_pushes) & empty & utils::MASK_RANK[3];
  }

  u64 mask_black_pawn_double_pushes(u64 bpawns, u64 empty) noexcept
  {
    u64 single_pushes = mask_black_pawn_single_pushes(bpawns, empty);
    return bitboard::sout_one(single_pushes) & empty & utils::MASK_RANK[4];
  }

  u64 mask_white_pawn_any_attacks(u64 wpawns) noexcept { return mask_white_pawn_east_attacks(wpawns) | mask_white_pawn_west_attacks(wpawns); }

  u64 mask_black_pawn_any_attacks(u64 bpawns) noexcept { return mask_black_pawn_east_attacks(bpawns) | mask_black_pawn_west_attacks(bpawns); }

  u64 mask_knight_attacks(u64 knights) noexcept 
  {
    constexpr u64 CLEAR_FILE_HG = 0x3f3f3f3f3f3f3f3f;
    constexpr u64 CLEAR_FILE_AB = 0xfcfcfcfcfcfcfcfc;

    u64 l1 = (knights >> 1) & utils::MASK_CLEAR_FILE[7];
    u64 r1 = (knights << 1) & utils::MASK_CLEAR_FILE[0];
    u64 h1 = l1 | r1;

    u64 l2 = (knights >> 2) & CLEAR_FILE_HG;
    u64 r2 = (knights << 2) & CLEAR_FILE_AB;
    u64 h2 = l2 | r2;

    return (h1 << 16) | (h1 >> 16) | (h2 << 8) | (h2 >> 8);
  }

  u64 mask_king_attacks(u64 kings) noexcept
  {
    u64 attacks = bitboard::east_one(kings) | bitboard::west_one(kings);
    kings |= attacks;
    attacks |= bitboard::nort_one(kings) | bitboard::sout_one(kings);
    return attacks;
  }

  u64 mask_bishop_attack_rays(Square sq) noexcept
  {
    u64 attacks = bitboard::kZERO;
    Rank rank = utils::get_rank(sq);
    File file = utils::get_file(sq);
    for (int r = rank + 1, f = file + 1; r < RANK_8 && f < FILE_H; r++, f++)
    {
      attacks |= (bitboard::kONE << utils::get_square((Rank)r, (File)f));
    }

    for (int r = rank + 1, f = file - 1; r < RANK_8 && f > FILE_A; r++, f--)
    {
      attacks |= (bitboard::kONE << utils::get_square((Rank)r, (File)f));
    }

    for (int r = rank - 1, f = file + 1; r > RANK_1 && f < FILE_H; r--, f++)
    {
      attacks |= (bitboard::kONE << utils::get_square((Rank)r, (File)f));
    }

    for (int r = rank - 1, f = file - 1; r > RANK_1 && f > FILE_A; r--, f--)
    {
      attacks |= (bitboard::kONE << utils::get_square((Rank)r, (File)f));
    }

    return attacks;
  }

  u64 mask_rook_attack_rays(Square sq) noexcept
  {
    u64 attacks = bitboard::kZERO;
    Rank rank = utils::get_rank(sq);
    File file = utils::get_file(sq);
    for (int r = rank + 1; r < RANK_8; r++)
    {
      attacks |= (bitboard::kONE << utils::get_square((Rank)r, file));
    }

    for (int r = rank - 1; r > RANK_1; r--)
    {
      attacks |= (bitboard::kONE << utils::get_square((Rank)r, file));
    }

    for (int f = file + 1; f < FILE_H; f++)
    {
      attacks |= (bitboard::kONE << utils::get_square(rank, (File)f));
    }

    for (int f = file - 1; f > FILE_A; f--)
    {
      attacks |= (bitboard::kONE << utils::get_square(rank, (File)f));
    }

    return attacks;
  }

  u64 mask_bishop_xray_attacks(Square sq, u64 block) noexcept
  {
    u64 attacks = bitboard::kZERO;
    Rank rank = utils::get_rank(sq);
    File file = utils::get_file(sq);
    for (int r = rank + 1, f = file + 1; r < N_RANKS && f < N_FILES; r++, f++)
    {
      attacks |= (bitboard::kONE << utils::get_square((Rank)r, (File)f));
      if ((bitboard::kONE << utils::get_square((Rank)r, (File)f)) & block)
      {
        break;
      }
    }

    for (int r = rank + 1, f = file - 1; r < N_RANKS && f >= FILE_A; r++, f--)
    {
      attacks |= (bitboard::kONE << utils::get_square((Rank)r, (File)f));
      if ((bitboard::kONE << utils::get_square((Rank)r, (File)f)) & block)
      {
        break;
      }
    }

    for (int r = rank - 1, f = file + 1; r >= RANK_1 && f < N_FILES; r--, f++)
    {
      attacks |= (bitboard::kONE << utils::get_square((Rank)r, (File)f));
      if ((bitboard::kONE << utils::get_square((Rank)r, (File)f)) & block)
      {
        break;
      }
    }

    for (int r = rank - 1, f = file - 1; r >= RANK_1 && f >= FILE_A; r--, f--)
    {
      attacks |= (bitboard::kONE << utils::get_square((Rank)r, (File)f));
      if ((bitboard::kONE << utils::get_square((Rank)r, (File)f)) & block)
      {
        break;
      }
    }

    return attacks;
  }

  u64 mask_rook_xray_attacks(Square sq, u64 block) noexcept
  {
    u64 attacks = bitboard::kZERO;
    Rank rank = utils::get_rank(sq);
    File file = utils::get_file(sq);
    for (int r = rank + 1; r < N_RANKS; r++)
    {
      attacks |= (bitboard::kONE << utils::get_square((Rank)r, file));
      if ((bitboard::kONE << utils::get_square((Rank)r, file)) & block)
      {
        break;
      }
    }

    for (int r = rank - 1; r >= RANK_1; r--)
    {
      attacks |= (bitboard::kONE << utils::get_square((Rank)r, file));
      if ((bitboard::kONE << utils::get_square((Rank)r, file)) & block)
      {
        break;
      }
    }

    for (int f = file + 1; f < N_FILES; f++)
    {
      attacks |= (bitboard::kONE << utils::get_square(rank, (File)f));
      if ((bitboard::kONE << utils::get_square(rank, (File)f)) & block)
      {
        break;
      }
    }

    for (int f = file - 1; f >= FILE_A; f--)
    {
      attacks |= (bitboard::kONE << utils::get_square(rank, (File)f));
      if ((bitboard::kONE << utils::get_square(rank, (File)f)) & block)
      {
        break;
      }
    }

    return attacks;
  }
} // namespace attacks