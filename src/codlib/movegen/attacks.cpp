#include <codlib/movegen/attacks.hpp>

#include <codlib/bitboard.hpp>
#include <codlib/utils.hpp>

using bitboard::Bitboard;

namespace attacks {

Bitboard ATTACKS_KING[N_SQUARES];

Bitboard ATTACKS_KNIGHT[N_SQUARES];

Bitboard ATTACKS_PAWN[N_SIDES][N_SQUARES];

/// @brief Bitboard masks for each rook attack
/// @note The init() function must be called before using this
/// @note The function get_rook_attacks() should be used to get the actual attacks
auto ATTACKS_ROOK = std::vector<std::vector<Bitboard>>(N_SQUARES, std::vector<Bitboard>(N_ROOK_MAGICS));

/// @brief Bitboard masks for each bishop attack
/// @note The init() function must be called before using this
/// @note The function get_bishop_attacks() should be used to get the actual attacks
auto ATTACKS_BISHOP = std::vector<std::vector<Bitboard>>(N_SQUARES, std::vector<Bitboard>(N_BISHOP_MAGICS));

/// @brief Returns a bitboard of all the knight attacks.
/// @param knights A bitboard of knights.
/// @return A bitboard of all the knight attacks.
[[nodiscard]] constexpr Bitboard mask_knight_attacks(const Bitboard knights) noexcept
{
  constexpr Bitboard CLEAR_FILE_HG = 0x3f3f3f3f3f3f3f3f;
  constexpr Bitboard CLEAR_FILE_AB = 0xfcfcfcfcfcfcfcfc;

  Bitboard l1 = (knights >> 1) & utils::MASK_CLEAR_FILE[7];
  Bitboard r1 = (knights << 1) & utils::MASK_CLEAR_FILE[0];
  Bitboard h1 = l1 | r1;

  Bitboard l2 = (knights >> 2) & CLEAR_FILE_HG;
  Bitboard r2 = (knights << 2) & CLEAR_FILE_AB;
  Bitboard h2 = l2 | r2;

  return (h1 << 16) | (h1 >> 16) | (h2 << 8) | (h2 >> 8);
}

/// @brief Returns a bitboard of all the king attacks.
/// @param kings A bitboard of kings.
/// @return A bitboard of all the king attacks.
[[nodiscard]] constexpr Bitboard mask_king_attacks(Bitboard kings) noexcept
{
  Bitboard attacks = bitboard::east_one(kings) | bitboard::west_one(kings);
  kings |= attacks;
  attacks |= bitboard::nort_one(kings) | bitboard::sout_one(kings);
  return attacks;
}

void init() noexcept
{
  // Initializing Leaper Piece Attack Tables
  for (int sq = A1; sq < N_SQUARES; ++sq) {
    ATTACKS_PAWN[WHITE][sq] = attacks::mask_white_pawn_any_attacks(utils::SQUARE_BB[sq]);
    ATTACKS_PAWN[BLACK][sq] = attacks::mask_black_pawn_any_attacks(utils::SQUARE_BB[sq]);
  }

  for (int sq = A1; sq < N_SQUARES; ++sq) { ATTACKS_KNIGHT[sq] = attacks::mask_knight_attacks(utils::SQUARE_BB[sq]); }

  for (int sq = A1; sq < N_SQUARES; ++sq) { ATTACKS_KING[sq] = attacks::mask_king_attacks(utils::SQUARE_BB[sq]); }

  // Initialize Slider Piece Attack Tables
  for (int sq = A1; sq < N_SQUARES; ++sq) {
    int occupancy_indices = 1 << utils::RELEVANT_BITS_COUNT_BISHOP[sq];
    for (int i = 0; i < occupancy_indices; ++i) {
      magics::Magic magic = magics::MAGIC_TABLE_BISHOP[sq];
      Bitboard occupancy = bitboard::set_occupancy(i, utils::RELEVANT_BITS_COUNT_BISHOP[sq], magic.mask);
      int index = static_cast<int>((occupancy * magic.magic) >> magic.shift);
      ATTACKS_BISHOP[sq][index] = attacks::mask_bishop_xray_attacks((Square)sq, occupancy);
    }
  }

  for (int sq = A1; sq < N_SQUARES; ++sq) {
    int occupancy_indices = 1 << utils::RELEVANT_BITS_COUNT_ROOK[sq];
    for (int i = 0; i < occupancy_indices; ++i) {
      magics::Magic magic = magics::MAGIC_TABLE_ROOK[sq];
      Bitboard occupancy = bitboard::set_occupancy(i, utils::RELEVANT_BITS_COUNT_ROOK[sq], magic.mask);
      int index = (int)((occupancy * magic.magic) >> magic.shift);
      ATTACKS_ROOK[sq][index] = attacks::mask_rook_xray_attacks((Square)sq, occupancy);
    }
  }
}

Bitboard get_bishop_attacks(const Square sq, const Bitboard occ) noexcept
{
  const auto idx = magics::magic_index(occ, magics::MAGIC_TABLE_BISHOP[sq]);
  return ATTACKS_BISHOP[sq][idx];
}

Bitboard get_rook_attacks(const Square sq, const Bitboard occ) noexcept
{
  const auto idx = magics::magic_index(occ, magics::MAGIC_TABLE_ROOK[sq]);
  return ATTACKS_ROOK[sq][idx];
}

}// namespace attacks