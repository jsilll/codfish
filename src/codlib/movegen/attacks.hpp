#pragma once

#include <vector>

#include <codlib/bitboard.hpp>
#include <codlib/movegen/magics.hpp>
#include <codlib/utils.hpp>

namespace attacks {
/// @brief Initializes all the attack tables
void init() noexcept;

/// @brief Number of rook magics
/// @note 4096 is the number of unique 12-bit numbers
constexpr std::size_t N_ROOK_MAGICS = 4096;

/// @brief Number of bishop magics
/// @note 512 is the number of unique 9-bit numbers
constexpr std::size_t N_BISHOP_MAGICS = 512;

/// @brief Bitboard masks for each king attack
/// @note The init() function must be called before using this
extern bitboard::u64 ATTACKS_KING[N_SQUARES];

/// @brief Bitboard masks for each knight attack
/// @note The init() function must be called before using this
extern bitboard::u64 ATTACKS_KNIGHT[N_SQUARES];

/// @brief Bitboard masks for each pawn attack
/// @note The init() function must be called before using this
extern bitboard::u64 ATTACKS_PAWN[N_SIDES][N_SQUARES];

/// @brief Returns a bitboard of all east attacks of white pawns.
/// @param wpawns A bitboard of white pawns.
/// @return A bitboard of all east attacks of white pawns.
[[nodiscard]] constexpr bitboard::u64 mask_white_pawn_east_attacks(bitboard::u64 wpawns) noexcept
{
  return bitboard::no_ea_one(wpawns);
}

/// @brief Returns a bitboard of all west attacks of white pawns.
/// @param wpawns A bitboard of white pawns.
/// @return A bitboard of all west attacks of white pawns.
[[nodiscard]] constexpr bitboard::u64 mask_white_pawn_west_attacks(bitboard::u64 wpawns) noexcept
{
  return bitboard::no_we_one(wpawns);
}

/// @brief Returns a bitboard of all east attacks of black pawns.
/// @param bpawns A bitboard of black pawns.
/// @return A bitboard of all east attacks of black pawns.
[[nodiscard]] constexpr bitboard::u64 mask_black_pawn_east_attacks(bitboard::u64 bpawns) noexcept
{
  return bitboard::so_ea_one(bpawns);
}

/// @brief Returns a bitboard of all west attacks of black pawns.
/// @param bpawns A bitboard of black pawns.
/// @return A bitboard of all west attacks of black pawns.
[[nodiscard]] constexpr bitboard::u64 mask_black_pawn_west_attacks(bitboard::u64 bpawns) noexcept
{
  return bitboard::so_we_one(bpawns);
}

/// @brief Returns a bitboard of all white pawn single pushes.
/// @param wpawns A bitboard of white pawns.
/// @param empty A bitboard of empty squares.
/// @return A bitboard of all white pawn single pushes.
[[nodiscard]] constexpr bitboard::u64 mask_white_pawn_single_pushes(bitboard::u64 wpawns, bitboard::u64 empty) noexcept
{
  return bitboard::nort_one(wpawns) & empty;
}

/// @brief Returns a bitboard of all black pawn single pushes.
/// @param bpawns A bitboard of black pawns.
/// @param empty A bitboard of empty squares.
/// @return A bitboard of all black pawn single pushes.
[[nodiscard]] constexpr bitboard::u64 mask_black_pawn_single_pushes(bitboard::u64 bpawns, bitboard::u64 empty) noexcept
{
  return bitboard::sout_one(bpawns) & empty;
}

/// @brief Returns a bitboard of all white pawn double pushes.
/// @param wpawns A bitboard of white pawns.
/// @param empty A bitboard of empty squares.
/// @return A bitboard of all white pawn double pushes.
[[nodiscard]] constexpr bitboard::u64 mask_white_pawn_double_pushes(bitboard::u64 wpawns, bitboard::u64 empty) noexcept
{
  bitboard::u64 single_pushes = mask_white_pawn_single_pushes(wpawns, empty);
  return bitboard::nort_one(single_pushes) & empty & utils::MASK_RANK[RANK_3];
}

/// @brief Returns a bitboard of all black pawn double pushes.
/// @param bpawns A bitboard of black pawns.
/// @param empty A bitboard of empty squares.
/// @return A bitboard of all black pawn double pushes.
[[nodiscard]] constexpr bitboard::u64 mask_black_pawn_double_pushes(bitboard::u64 bpawns, bitboard::u64 empty) noexcept
{
  bitboard::u64 single_pushes = mask_black_pawn_single_pushes(bpawns, empty);
  return bitboard::sout_one(single_pushes) & empty & utils::MASK_RANK[RANK_4];
}

/// @brief Returns a bitboard of all the white pawn attacks.
/// @param wpawns A bitboard of white pawns.
/// @return A bitboard of all the white pawn attacks.
[[nodiscard]] constexpr bitboard::u64 mask_white_pawn_any_attacks(bitboard::u64 wpawns) noexcept
{
  return mask_white_pawn_east_attacks(wpawns) | mask_white_pawn_west_attacks(wpawns);
}

/// @brief Returns a bitboard of all the black pawn attacks.
/// @param bpawns A bitboard of black pawns.
/// @return A bitboard of all the black pawn attacks.
[[nodiscard]] constexpr bitboard::u64 mask_black_pawn_any_attacks(bitboard::u64 bpawns) noexcept
{
  return mask_black_pawn_east_attacks(bpawns) | mask_black_pawn_west_attacks(bpawns);
}

/// @brief Returns a bitboard of all the bishop attacks from a given square with
/// a given block.
/// @param sq The square to generate attacks from.
/// @param block A bitboard of the blocked squares.
/// @return A bitboard of all the bishop attacks.
[[nodiscard]] constexpr bitboard::u64 mask_bishop_xray_attacks(Square sq, bitboard::u64 block) noexcept
{
  bitboard::u64 attacks = bitboard::kZERO;
  Rank rank = utils::get_rank(sq);
  File file = utils::get_file(sq);
  for (int r = rank + 1, f = file + 1; r < N_RANKS && f < N_FILES; ++r, ++f) {
    attacks |= (bitboard::kONE << utils::get_square((Rank)r, (File)f));
    if ((bitboard::kONE << utils::get_square((Rank)r, (File)f)) & block) { break; }
  }

  for (int r = rank + 1, f = file - 1; r < N_RANKS && f >= FILE_A; ++r, --f) {
    attacks |= (bitboard::kONE << utils::get_square((Rank)r, (File)f));
    if ((bitboard::kONE << utils::get_square((Rank)r, (File)f)) & block) { break; }
  }

  for (int r = rank - 1, f = file + 1; r >= RANK_1 && f < N_FILES; --r, ++f) {
    attacks |= (bitboard::kONE << utils::get_square((Rank)r, (File)f));
    if ((bitboard::kONE << utils::get_square((Rank)r, (File)f)) & block) { break; }
  }

  for (int r = rank - 1, f = file - 1; r >= RANK_1 && f >= FILE_A; --r, --f) {
    attacks |= (bitboard::kONE << utils::get_square((Rank)r, (File)f));
    if ((bitboard::kONE << utils::get_square((Rank)r, (File)f)) & block) { break; }
  }

  return attacks;
}

/// @brief Returns a bitboard of all the rook attacks from a given square with a
/// given block.
/// @param sq The square to generate attacks from.
/// @param block A bitboard of the blocked squares.
/// @return A bitboard of all the rook attacks.
[[nodiscard]] constexpr bitboard::u64 mask_rook_xray_attacks(Square sq, bitboard::u64 block) noexcept
{
  bitboard::u64 attacks = bitboard::kZERO;
  Rank rank = utils::get_rank(sq);
  File file = utils::get_file(sq);
  for (int r = rank + 1; r < N_RANKS; ++r) {
    attacks |= (bitboard::kONE << utils::get_square((Rank)r, file));
    if ((bitboard::kONE << utils::get_square((Rank)r, file)) & block) { break; }
  }

  for (int r = rank - 1; r >= RANK_1; --r) {
    attacks |= (bitboard::kONE << utils::get_square((Rank)r, file));
    if ((bitboard::kONE << utils::get_square((Rank)r, file)) & block) { break; }
  }

  for (int f = file + 1; f < N_FILES; ++f) {
    attacks |= (bitboard::kONE << utils::get_square(rank, (File)f));
    if ((bitboard::kONE << utils::get_square(rank, (File)f)) & block) { break; }
  }

  for (int f = file - 1; f >= FILE_A; --f) {
    attacks |= (bitboard::kONE << utils::get_square(rank, (File)f));
    if ((bitboard::kONE << utils::get_square(rank, (File)f)) & block) { break; }
  }

  return attacks;
}

/// @brief Gets the bishop attacks for a given square and occupancy
/// @param sq The square
/// @param occ The occupancy
/// @return The bishop attacks
[[nodiscard]] bitboard::u64 get_bishop_attacks(Square sq, bitboard::u64 occ) noexcept;

/// @brief Gets the rook attacks for a given square and occupancy
/// @param sq The square
/// @param occ The occupancy
/// @return The rook attacks
[[nodiscard]] bitboard::u64 get_rook_attacks(Square sq, bitboard::u64 occ) noexcept;

/// @brief Gets the queen attacks for a given square and occupancy
/// @param sq The square
/// @param occ The occupancy
/// @return The queen attacks
[[nodiscard]] bitboard::u64 get_queen_attacks(const Square sq, bitboard::u64 occ) noexcept
{
  return get_bishop_attacks(sq, occ) | get_rook_attacks(sq, occ);
}
}// namespace attacks