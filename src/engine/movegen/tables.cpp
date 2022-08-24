#include <engine/movegen/tables.hpp>

#include <engine/bitboard.hpp>
#include <engine/movegen/magics.hpp>
#include <engine/movegen/attacks.hpp>

namespace tables
{
  u64 SQUARE_BB[N_SQUARES];

  // clang-format off
    static const int RELEVANT_BITS_COUNT_BISHOP[N_SQUARES] = {
        6, 5, 5, 5, 5, 5, 5, 6,
        5, 5, 5, 5, 5, 5, 5, 5,
        5, 5, 7, 7, 7, 7, 5, 5,
        5, 5, 7, 9, 9, 7, 5, 5,
        5, 5, 7, 9, 9, 7, 5, 5,
        5, 5, 7, 7, 7, 7, 5, 5,
        5, 5, 5, 5, 5, 5, 5, 5,
        6, 5, 5, 5, 5, 5, 5, 6,
    };
  // clang-format on

  // clang-format off
    static const int RELEVANT_BITS_COUNT_ROOK[N_SQUARES] = {
        12, 11, 11, 11, 11, 11, 11, 12,
        11, 10, 10, 10, 10, 10, 10, 11,
        11, 10, 10, 10, 10, 10, 10, 11,
        11, 10, 10, 10, 10, 10, 10, 11,
        11, 10, 10, 10, 10, 10, 10, 11,
        11, 10, 10, 10, 10, 10, 10, 11,
        11, 10, 10, 10, 10, 10, 10, 11,
        12, 11, 11, 11, 11, 11, 11, 12,
    };
  // clang-format on

  static auto ATTACKS_PAWN = new u64[N_SIDES][N_SQUARES];
  static u64 *ATTACKS_KNIGHT = new u64[N_SQUARES];
  static u64 *ATTACKS_KING = new u64[N_SQUARES];
  static auto ATTACKS_BISHOP = new u64[N_SQUARES][512];
  static auto ATTACKS_ROOK = new u64[N_SQUARES][4096];

  u64 square_to_bitboard(Square sq)
  {
    return SQUARE_BB[sq];
  }

  int get_relevant_bits_count_bishop(Square sq)
  {
    return RELEVANT_BITS_COUNT_BISHOP[sq];
  }

  int get_relevant_bits_count_rook(Square sq)
  {
    return RELEVANT_BITS_COUNT_ROOK[sq];
  }

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
        magics::Magic magic = magics::get_bishop_magic((Square)sq);
        u64 occupancy = bitboard::set_occupancy(i, RELEVANT_BITS_COUNT_BISHOP[sq], magic.mask);
        int index = (int)((occupancy * magic.magic) >> magic.shift);
        ATTACKS_BISHOP[sq][index] = attacks::mask_bishop_xray_attacks((Square)sq, occupancy);
      }
    }

    for (int sq = A1; sq < N_SQUARES; sq++)
    {
      int occupancy_indices = 1 << RELEVANT_BITS_COUNT_ROOK[sq];
      for (int i = 0; i < occupancy_indices; i++)
      {
        magics::Magic magic = magics::get_rook_magic((Square)sq);
        u64 occupancy = bitboard::set_occupancy(i, RELEVANT_BITS_COUNT_ROOK[sq], magic.mask);
        int index = (int)((occupancy * magic.magic) >> magic.shift);
        ATTACKS_ROOK[sq][index] = attacks::mask_rook_xray_attacks((Square)sq, occupancy);
      }
    }
  }

  void teardown()
  {
    delete[] ATTACKS_PAWN;
    delete[] ATTACKS_BISHOP;
    delete[] ATTACKS_ROOK;
  }

  u64 get_pawn_attacks(Color color, Square sq)
  {
    return ATTACKS_PAWN[color][sq];
  }

  u64 get_knight_attacks(Square sq)
  {
    return ATTACKS_KNIGHT[sq];
  }

  u64 get_king_attacks(Square sq)
  {
    return ATTACKS_KING[sq];
  }

  u64 get_bishop_attacks(const Square sq, u64 occ)
  {
    magics::Magic magic = magics::get_bishop_magic(sq);
    occ &= magic.mask;
    occ *= magic.magic;
    occ >>= magic.shift;
    return ATTACKS_BISHOP[sq][occ];
  }

  u64 get_rook_attacks(const Square sq, u64 occ)
  {
    magics::Magic magic = magics::get_rook_magic(sq);
    occ &= magic.mask;
    occ *= magic.magic;
    occ >>= magic.shift;
    return ATTACKS_ROOK[sq][occ];
  }

  u64 get_queen_attacks(const Square sq, u64 occ)
  {
    return get_bishop_attacks(sq, occ) | get_rook_attacks(sq, occ);
  }

} // namespace tables