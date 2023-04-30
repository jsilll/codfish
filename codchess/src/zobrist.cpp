#include "zobrist.hpp"

namespace codchess::zobrist {
/// @brief The keys for the side to move
bitboard::Bitboard SIDE_KEY[BOTH]{};

/// @brief The keys for the en passant squares
bitboard::Bitboard EN_PASSANT_KEY[N_SQUARES];

/// @brief The keys for the castling_availability rights
bitboard::Bitboard CASTLE_KEY[N_CASTLING_KEYS];

/// @brief The keys for the pieces
bitboard::Bitboard PIECE_KEY[N_COLORS][N_PIECES][N_SQUARES];

/// @brief Generates a random number
/// @return The random number
bitboard::Bitboard
RandomBitboard() noexcept {
    const auto n1 = (static_cast<bitboard::Bitboard>(std::rand()));
    const auto n2 = (static_cast<bitboard::Bitboard>(std::rand()));
    return n1 | (n2 << 32);
}

void
Init() noexcept {
    for (auto &en_passant_key : EN_PASSANT_KEY) {
        en_passant_key = RandomBitboard();
    }

    for (auto &castle_key : CASTLE_KEY) {
        castle_key = RandomBitboard();
    }

    for (int piece = PAWN; piece < N_PIECES; ++piece) {
        for (int square = A1; square < N_SQUARES; ++square) {
            PIECE_KEY[WHITE][piece][square] = RandomBitboard();
            PIECE_KEY[BLACK][piece][square] = RandomBitboard();
        }
    }

    SIDE_KEY[1] = RandomBitboard();
}

bitboard::Bitboard
Hash(const Board &board) noexcept {
    auto final_key{bitboard::ZERO};
    for (int piece = PAWN; piece < N_PIECES; ++piece) {
        for (int side = WHITE; side < BOTH; ++side) {
            auto bitboard = board.pieces(static_cast<Color>(side),
                                         static_cast<PieceType>(piece));
            while (bitboard) {
                const auto sq = bitboard::BitScanForward(bitboard);
                final_key ^= PIECE_KEY[side][piece][sq];
                bitboard::PopBit(bitboard, sq);
            }
        }
    }

    if (board.en_passant_square() != EMPTY_SQUARE) {
        final_key ^= EN_PASSANT_KEY[board.en_passant_square()];
    }

    final_key ^= CASTLE_KEY[board.castling_availability()];
    final_key ^= SIDE_KEY[board.active()];

    return final_key;
}
}   // namespace codchess::zobrist