#include "zobrist.hpp"

namespace cod::chess::zobrist {
/// @brief The keys for the side to move
bitboard::Bitboard SIDE_KEY[static_cast<std::size_t>(Color::BOTH)]{};

/// @brief The keys for the en passant squares
bitboard::Bitboard EN_PASSANT_KEY[static_cast<std::size_t>(Square::N_SQUARES)];

/// @brief The keys for the castling_availability rights
bitboard::Bitboard CASTLE_KEY[N_CASTLING_KEYS];

/// @brief The keys for the pieces
bitboard::Bitboard PIECE_KEY[static_cast<std::size_t>(Color::N_COLORS)]
                            [static_cast<std::size_t>(Piece::N_PIECES)]
                            [static_cast<std::size_t>(Square::N_SQUARES)];

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

    for (int piece = static_cast<int>(Piece::PAWN);
         piece < static_cast<int>(Piece::N_PIECES); ++piece) {
        for (int square = static_cast<int>(Square::A1);
             square < static_cast<int>(Square::N_SQUARES); ++square) {
            PIECE_KEY[static_cast<std::size_t>(Color::WHITE)][piece][square] =
                RandomBitboard();
            PIECE_KEY[static_cast<std::size_t>(Color::BLACK)][piece][square] =
                RandomBitboard();
        }
    }

    SIDE_KEY[1] = RandomBitboard();
}

bitboard::Bitboard
Hash(const Board &board) noexcept {
    auto final_key{bitboard::ZERO};
    for (int piece = static_cast<int>(Piece::PAWN);
         piece < static_cast<int>(Piece::N_PIECES); ++piece) {
        for (int side = static_cast<int>(Color::WHITE);
             side < static_cast<int>(Color::BOTH); ++side) {
            auto bitboard = board.pieces(static_cast<Color>(side),
                                         static_cast<Piece>(piece));
            while (bitboard) {
                const auto sq = bitboard::BitScanForward(bitboard);
                final_key ^=
                    PIECE_KEY[side][piece][static_cast<std::size_t>(sq)];
                bitboard::PopBit(bitboard, sq);
            }
        }
    }

    if (board.en_passant_square() != Square::EMPTY_SQUARE) {
        final_key ^=
            EN_PASSANT_KEY[static_cast<std::size_t>(board.en_passant_square())];
    }

    final_key ^=
        CASTLE_KEY[static_cast<std::size_t>(board.castling_availability())];
    final_key ^= SIDE_KEY[static_cast<std::size_t>(board.active())];

    return final_key;
}
}   // namespace cod::chess::zobrist