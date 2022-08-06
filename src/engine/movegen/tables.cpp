#include <engine/movegen/tables.hpp>

#include <engine/bitboard.hpp>
#include <engine/movegen/magics.hpp>
#include <engine/movegen/attacks.hpp>

namespace tables
{
  u64 SQUARE_BB[N_SQUARES];
  u64 ATTACKS_PAWN[N_SIDES][N_SQUARES];
  u64 ATTACKS_KNIGHT[N_SQUARES];
  u64 ATTACKS_KING[N_SQUARES];
  u64 ATTACKS_BISHOP[N_SQUARES][512];
  u64 ATTACKS_ROOK[N_SQUARES][4096];

  void init()
  {
    // Misc. Tables
    for (int sq = A1; sq < N_SQUARES; sq++)
    {
      SQUARE_BB[sq] = ONE << sq;
    }

    // Initializing Leaper Piece Attack Tables
    for (int sq = A1; sq < N_SQUARES; sq++)
    {
      ATTACKS_PAWN[WHITE][sq] = attacks::mask_white_pawn_any_attacks(SQUARE_BB[sq]);
      ATTACKS_PAWN[BLACK][sq] = attacks::mask_black_pawn_any_attacks(SQUARE_BB[sq]);
    }

    for (int sq = A1; sq < N_SQUARES; sq++)
    {
      ATTACKS_KNIGHT[sq] = attacks::mask_knight_attacks(SQUARE_BB[sq]);
    }

    for (int sq = A1; sq < N_SQUARES; sq++)
    {
      ATTACKS_KING[sq] = attacks::mask_king_attacks(SQUARE_BB[sq]);
    }

    // Initialize Slider Piece Attack Tables
    for (int sq = A1; sq < N_SQUARES; sq++)
    {
      int occupancy_indices = 1 << RELEVANT_BITS_COUNT_BISHOP[sq];
      for (int i = 0; i < occupancy_indices; i++)
      {
        u64 occupancy = bitboard::set_occupancy(i, RELEVANT_BITS_COUNT_BISHOP[sq], magics::MAGIC_TABLE_BISHOP[sq].mask);
        int magic = (int)((occupancy * magics::MAGIC_TABLE_BISHOP[sq].magic) >> magics::MAGIC_TABLE_BISHOP[sq].shift);
        ATTACKS_BISHOP[sq][magic] = attacks::mask_bishop_attacks(sq, occupancy);
      }
    }

    for (int sq = A1; sq < N_SQUARES; sq++)
    {
      int occupancy_indices = 1 << RELEVANT_BITS_COUNT_ROOK[sq];
      for (int i = 0; i < occupancy_indices; i++)
      {
        u64 occupancy = bitboard::set_occupancy(i, RELEVANT_BITS_COUNT_ROOK[sq], magics::MAGIC_TABLE_ROOK[sq].mask);
        int magic = (int)((occupancy * magics::MAGIC_TABLE_ROOK[sq].magic) >> magics::MAGIC_TABLE_ROOK[sq].shift);
        ATTACKS_ROOK[sq][magic] = attacks::mask_rook_attacks(sq, occupancy);
      }
    }
  }

} // namespace tables