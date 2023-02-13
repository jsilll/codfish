#include <codlib/movegen/attacks.hpp>

#include <codlib/utils.hpp>
#include <codlib/bitboard.hpp>
#include <codlib/movegen/attacks.hpp>

namespace attacks
{
  u64 ATTACKS_KING[N_SQUARES];

  u64 ATTACKS_KNIGHT[N_SQUARES];

  u64 ATTACKS_PAWN[N_SIDES][N_SQUARES];

  /// @brief Bitboard masks for each rook attack
  /// @note The init() function must be called before using this
  /// @note The function get_rook_attacks() should be used to get the actual attacks
  auto ATTACKS_ROOK = std::vector<std::vector<u64>>(N_SQUARES, std::vector<u64>(N_ROOK_MAGICS));

  /// @brief Bitboard masks for each bishop attack
  /// @note The init() function must be called before using this
  /// @note The function get_bishop_attacks() should be used to get the actual attacks
  auto ATTACKS_BISHOP = std::vector<std::vector<u64>>(N_SQUARES, std::vector<u64>(N_BISHOP_MAGICS));

  /// @brief Returns a bitboard of all the knight attacks.
  /// @param knights A bitboard of knights.
  /// @return A bitboard of all the knight attacks.
  [[nodiscard]] constexpr u64 mask_knight_attacks(u64 knights) noexcept
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

  /// @brief Returns a bitboard of all the king attacks.
  /// @param kings A bitboard of kings.
  /// @return A bitboard of all the king attacks.
  [[nodiscard]] constexpr u64 mask_king_attacks(u64 kings) noexcept
  {
    u64 attacks = bitboard::east_one(kings) | bitboard::west_one(kings);
    kings |= attacks;
    attacks |= bitboard::nort_one(kings) | bitboard::sout_one(kings);
    return attacks;
  }

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

  /// @brief Computes the magic index for a given occupancy and magic entry
  /// @param occ The occupancy
  /// @param magic The magic entry
  /// @return The magic index
  [[nodiscard]] constexpr u64 magic_index(u64 occ, const magics::Magic &magic) noexcept
  {
    occ &= magic.mask;
    occ *= magic.magic;
    occ >>= magic.shift;
    return occ;
  }

  u64 get_bishop_attacks(const Square sq, u64 occ) noexcept
  {
    const int idx = magic_index(occ, magics::MAGIC_TABLE_BISHOP[sq]);
    return ATTACKS_BISHOP[sq][idx];
  }

  u64 get_rook_attacks(const Square sq, u64 occ) noexcept
  {
    magics::Magic magic = magics::MAGIC_TABLE_ROOK[sq];
    occ &= magic.mask;
    occ *= magic.magic;
    occ >>= magic.shift;
    return ATTACKS_ROOK[sq][occ];
  }
} // namespace attacks