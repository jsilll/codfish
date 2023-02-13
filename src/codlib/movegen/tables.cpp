#include <codlib/movegen/tables.hpp>

#include <codlib/utils.hpp>
#include <codlib/bitboard.hpp>
#include <codlib/movegen/magics.hpp>
#include <codlib/movegen/attacks.hpp>

namespace tables
{
  void init() noexcept
  {
    // Initializing Leaper Piece Attack Tables
    for (int sq = A1; sq < N_SQUARES; sq++)
    {
      ATTACKS_PAWN[WHITE][sq] = attacks::mask_white_pawn_any_attacks(utils::SQUARE_BB[sq]);
      ATTACKS_PAWN[BLACK][sq] = attacks::mask_black_pawn_any_attacks(utils::SQUARE_BB[sq]);
    }

    for (int sq = A1; sq < N_SQUARES; sq++)
    {
      ATTACKS_KNIGHT[sq] = attacks::mask_knight_attacks(utils::SQUARE_BB[sq]);
    }

    for (int sq = A1; sq < N_SQUARES; sq++)
    {
      ATTACKS_KING[sq] = attacks::mask_king_attacks(utils::SQUARE_BB[sq]);
    }

    // Initialize Slider Piece Attack Tables
    for (int sq = A1; sq < N_SQUARES; sq++)
    {
      int occupancy_indices = 1 << utils::RELEVANT_BITS_COUNT_BISHOP[sq];
      for (int i = 0; i < occupancy_indices; i++)
      {
        magics::Magic magic = magics::MAGIC_TABLE_BISHOP[sq];
        u64 occupancy = bitboard::set_occupancy(i, utils::RELEVANT_BITS_COUNT_BISHOP[sq], magic.mask);
        int index = static_cast<int>((occupancy * magic.magic) >> magic.shift);
        ATTACKS_BISHOP[sq][index] = attacks::mask_bishop_xray_attacks((Square)sq, occupancy);
      }
    }

    for (int sq = A1; sq < N_SQUARES; sq++)
    {
      int occupancy_indices = 1 << utils::RELEVANT_BITS_COUNT_ROOK[sq];
      for (int i = 0; i < occupancy_indices; i++)
      {
        magics::Magic magic = magics::MAGIC_TABLE_ROOK[sq];
        u64 occupancy = bitboard::set_occupancy(i, utils::RELEVANT_BITS_COUNT_ROOK[sq], magic.mask);
        int index = (int)((occupancy * magic.magic) >> magic.shift);
        ATTACKS_ROOK[sq][index] = attacks::mask_rook_xray_attacks((Square)sq, occupancy);
      }
    }
  }
} // namespace tables