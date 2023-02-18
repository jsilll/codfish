#include <codlib/zobrist.hpp>

using bitboard::Bitboard;

namespace zobrist {
    /// @brief The keys for the side to move
    Bitboard SIDE_KEY[BOTH]{};

    /// @brief The keys for the en passant squares
    Bitboard EN_PASSANT_KEY[N_SQUARES];

    /// @brief The keys for the castling_availability rights
    Bitboard CASTLE_KEY[N_CASTLING_KEYS];

    /// @brief The keys for the pieces
    Bitboard PIECE_KEY[N_COLORS][N_PIECES][N_SQUARES];

    /// @brief Generates a random number
    /// @return The random number
    Bitboard RandomBitboard() noexcept {
        Bitboard n1 = (static_cast<Bitboard>(std::rand()));// NOLINT
        Bitboard n2 = (static_cast<Bitboard>(std::rand()));// NOLINT
        return n1 | (n2 << 32);
    }

    void Init() noexcept {
        for (auto &en_passant_key: EN_PASSANT_KEY) { en_passant_key = RandomBitboard(); }

        for (auto &castle_key: CASTLE_KEY) { castle_key = RandomBitboard(); }

        for (int piece = PAWN; piece < N_PIECES; piece++) {
            for (int square = A1; square < N_SQUARES; square++) {
                PIECE_KEY[WHITE][piece][square] = RandomBitboard();
                PIECE_KEY[BLACK][piece][square] = RandomBitboard();
            }
        }

        SIDE_KEY[1] = RandomBitboard();
    }

    Bitboard generate_hash_key(const Board &board) noexcept {
        Bitboard final_key = bitboard::ZERO;
        for (int piece = PAWN; piece < N_PIECES; ++piece) {
            for (int side = WHITE; side < BOTH; ++side) {
                Bitboard bitboard = board.pieces((Color) side, (PieceType) piece);
                while (bitboard) {
                    Square sq = bitboard::BitScanForward(bitboard);
                    final_key ^= PIECE_KEY[side][piece][sq];
                    bitboard::PopBit(bitboard, sq);
                }
            }
        }

        if (board.en_passant_square() != EMPTY_SQUARE) { final_key ^= EN_PASSANT_KEY[board.en_passant_square()]; }

        final_key ^= CASTLE_KEY[board.castling_availability()];
        final_key ^= SIDE_KEY[board.active()];

        return final_key;
    }
}// namespace zobrist