#include "tables.hpp"

#include "utils.hpp"
#include "magics.hpp"
#include "attacks.hpp"

namespace tables
{

  U64 SQUARE_BB[N_SQUARES];
  U64 ATTACKS_PAWN[BOTH][N_SQUARES];
  U64 ATTACKS_KNIGHT[N_SQUARES];
  U64 ATTACKS_KING[N_SQUARES];
  U64 ATTACKS_BISHOP[N_SQUARES][512];
  U64 ATTACKS_ROOK[N_SQUARES][4096];

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
      ATTACKS_PAWN[WHITE][sq] = attacks::maskWhitePawnAnyAttacks(SQUARE_BB[sq]);
      ATTACKS_PAWN[BLACK][sq] = attacks::maskBlackPawnAnyAttacks(SQUARE_BB[sq]);
    }

    for (int sq = A1; sq < N_SQUARES; sq++)
    {
      ATTACKS_KNIGHT[sq] = attacks::maskKnightAttacks(SQUARE_BB[sq]);
    }

    for (int sq = A1; sq < N_SQUARES; sq++)
    {
      ATTACKS_KING[sq] = attacks::maskKingAttacks(SQUARE_BB[sq]);
    }

    // Initialize Slider Piece Attack Tables
    for (int sq = A1; sq < N_SQUARES; sq++)
    {
      int occupancy_indices = 1 << RELEVANT_BITS_COUNT_BISHOP[sq];
      for (int i = 0; i < occupancy_indices; i++)
      {
        U64 occupancy = utils::setOccupancy(i, RELEVANT_BITS_COUNT_BISHOP[sq], magics::MAGIC_TABLE_BISHOP[sq].mask);
        int magic = (int)((occupancy * magics::MAGIC_TABLE_BISHOP[sq].magic) >> magics::MAGIC_TABLE_BISHOP[sq].shift);
        ATTACKS_BISHOP[sq][magic] = attacks::maskBishopAttacks(sq, occupancy);
      }
    }

    for (int sq = A1; sq < N_SQUARES; sq++)
    {
      int occupancy_indices = 1 << RELEVANT_BITS_COUNT_ROOK[sq];
      for (int i = 0; i < occupancy_indices; i++)
      {
        U64 occupancy = utils::setOccupancy(i, RELEVANT_BITS_COUNT_ROOK[sq], magics::MAGIC_TABLE_ROOK[sq].mask);
        int magic = (int)((occupancy * magics::MAGIC_TABLE_ROOK[sq].magic) >> magics::MAGIC_TABLE_ROOK[sq].shift);
        ATTACKS_ROOK[sq][magic] = attacks::maskRookAttacks(sq, occupancy);
      }
    }
  }

  U64 getBishopAttacks(const int &sq, U64 occ)
  {
    occ &= magics::MAGIC_TABLE_BISHOP[sq].mask;
    occ *= magics::MAGIC_TABLE_BISHOP[sq].magic;
    occ >>= magics::MAGIC_TABLE_BISHOP[sq].shift;
    return ATTACKS_BISHOP[sq][occ];
  }

  U64 getRookAttacks(const int &sq, U64 occ)
  {
    occ &= magics::MAGIC_TABLE_ROOK[sq].mask;
    occ *= magics::MAGIC_TABLE_ROOK[sq].magic;
    occ >>= magics::MAGIC_TABLE_ROOK[sq].shift;
    return ATTACKS_ROOK[sq][occ];
  }

  U64 getQueenAttacks(const int &sq, U64 occ)
  {
    return getBishopAttacks(sq, occ) | getRookAttacks(sq, occ);
  }

}