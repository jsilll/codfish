#include <codlib/zobrist.hpp>

using bitboard::u64;

namespace zobrist {
/// @brief The keys for the side to move
u64 side_key[BOTH]{};

/// @brief The keys for the en passant squares
u64 en_passant_keys[N_SQUARES];

/// @brief The keys for the castling rights
u64 castle_keys[N_CASTLING_KEYS];

/// @brief The keys for the pieces
u64 piece_keys[N_SIDES][N_PIECES][N_SQUARES];

/// @brief Generates a random number
/// @return The random number
u64 generate_random_number_u64() noexcept
{
  u64 n1 = (static_cast<u64>(std::rand()));// NOLINT
  u64 n2 = (static_cast<u64>(std::rand()));// NOLINT
  return n1 | (n2 << 32);
}

void init() noexcept
{
  for (auto &en_passant_key : en_passant_keys) { en_passant_key = generate_random_number_u64(); }

  for (auto &castle_key : castle_keys) { castle_key = generate_random_number_u64(); }

  for (int piece = PAWN; piece < N_PIECES; piece++) {
    for (int square = A1; square < N_SQUARES; square++) {
      piece_keys[WHITE][piece][square] = generate_random_number_u64();
      piece_keys[BLACK][piece][square] = generate_random_number_u64();
    }
  }

  side_key[1] = generate_random_number_u64();
}

bitboard::u64 generate_hash_key(const Board &board) noexcept
{
  bitboard::u64 final_key = bitboard::kZERO;
  for (int piece = PAWN; piece < N_PIECES; ++piece) {
    for (int side = WHITE; side < BOTH; ++side) {
      bitboard::u64 bitboard = board.pieces((Color) side, (PieceType) piece);
      while (bitboard) {
        Square sq = bitboard::bit_scan_forward(bitboard);
        final_key ^= piece_keys[side][piece][sq];
        bitboard::pop_bit(bitboard, sq);
      }
    }
  }

  if (board.en_passant_square() != EMPTY_SQUARE) { final_key ^= en_passant_keys[board.en_passant_square()]; }

  final_key ^= castle_keys[board.castling_availability()];
  final_key ^= side_key[board.active()];

  return final_key;
}
}// namespace zobrist