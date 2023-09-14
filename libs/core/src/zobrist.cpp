#include "zobrist.hpp"

namespace codfish::core::zobrist {
/// @brief The keys for the side to move
/// @note The cod::core::zobrist::Init() function must be called before using
std::uint64_t kSideKey[static_cast<std::size_t>(Color::Both)]{};

/// @brief The keys for the en passant squares
std::uint64_t kEnPassantKey[static_cast<std::size_t>(Square::Total)];

/// @brief The keys for the castling_availability rights
std::uint64_t kCastleKey[kCastlingKeys];

/// @brief The keys for the pieces
std::uint64_t kPieceKey[static_cast<std::size_t>(Color::Total)]
                            [static_cast<std::size_t>(Piece::Total)]
                            [static_cast<std::size_t>(Square::Total)];

/// @brief Generates a random number
/// @return The random number
std::uint64_t
RandomNumber() noexcept {
    const auto n1 = (static_cast<std::uint64_t>(std::rand()));
    const auto n2 = (static_cast<std::uint64_t>(std::rand()));
    return n1 | (n2 << 32);
}

void
Init() noexcept {
    for (auto &en_passant_key : kEnPassantKey) {
        en_passant_key = RandomNumber();
    }

    for (auto &castle_key : kCastleKey) {
        castle_key = RandomNumber();
    }

    for (int piece = static_cast<int>(Piece::Pawn);
         piece < static_cast<int>(Piece::Total); ++piece) {
        for (int square = static_cast<int>(Square::A1);
             square < static_cast<int>(Square::Total); ++square) {
            kPieceKey[static_cast<std::size_t>(Color::White)][piece][square] =
                RandomNumber();
            kPieceKey[static_cast<std::size_t>(Color::Black)][piece][square] =
                RandomNumber();
        }
    }

    kSideKey[1] = RandomNumber();
}

std::uint64_t
Hash(const Board &board) noexcept {
    std::uint64_t final_key{0};
    for (int piece = static_cast<int>(Piece::Pawn);
         piece < static_cast<int>(Piece::Total); ++piece) {
        for (int side = static_cast<int>(Color::White);
             side < static_cast<int>(Color::Both); ++side) {
            auto bitboard = board.pieces(static_cast<Color>(side),
                                         static_cast<Piece>(piece));
            while (bitboard) {
                const auto sq = bitboard::BitScanForward(bitboard);
                final_key ^=
                    kPieceKey[side][piece][static_cast<std::size_t>(sq)];
                bitboard::PopBit(bitboard, sq);
            }
        }
    }

    if (board.en_passant_square() != Square::Empty) {
        final_key ^=
            kEnPassantKey[static_cast<std::size_t>(board.en_passant_square())];
    }

    final_key ^=
        kCastleKey[static_cast<std::size_t>(board.castling_availability())];
    final_key ^= kSideKey[static_cast<std::size_t>(board.active())];

    return final_key;
}
}   // namespace codfish::core::zobrist