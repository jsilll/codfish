#include <codlib/zobrist.hpp>

using bitboard::Bitboard;

namespace zobrist {
/// @brief The keys for the side to move
    Bitboard side_key[BOTH]{};

/// @brief The keys for the en passant squares
    Bitboard en_passant_keys[N_SQUARES];

/// @brief The keys for the castling_availability rights
    Bitboard castle_keys[N_CASTLING_KEYS];

/// @brief The keys for the pieces
    Bitboard piece_keys[N_COLORS][N_PIECES][N_SQUARES];

/// @brief Generates a random number
/// @return The random number
    Bitboard generate_random_number_u64() noexcept {
        Bitboard n1 = (static_cast<Bitboard>(std::rand()));// NOLINT
        Bitboard n2 = (static_cast<Bitboard>(std::rand()));// NOLINT
        return n1 | (n2 << 32);
    }

    void init() noexcept {
        for (auto &en_passant_key: en_passant_keys) { en_passant_key = generate_random_number_u64(); }

        for (auto &castle_key: castle_keys) { castle_key = generate_random_number_u64(); }

        for (int piece = PAWN; piece < N_PIECES; piece++) {
            for (int square = A1; square < N_SQUARES; square++) {
                piece_keys[WHITE][piece][square] = generate_random_number_u64();
                piece_keys[BLACK][piece][square] = generate_random_number_u64();
            }
        }

        side_key[1] = generate_random_number_u64();
    }

    bitboard::Bitboard generate_hash_key(const Board &board) noexcept {
        // TODO: remove coupling with board
        bitboard::Bitboard final_key = bitboard::ZERO;
        for (int piece = PAWN; piece < N_PIECES; ++piece) {
            for (int side = WHITE; side < BOTH; ++side) {
                bitboard::Bitboard bitboard = board.pieces((Color) side, (PieceType) piece);
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