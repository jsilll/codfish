#pragma once

#include <core/base.hpp>
#include <core/bitboard.hpp>
#include <core/board.hpp>

namespace codfish::core::zobrist {
/// @brief Initializes the zobrist keys
void Init() noexcept;

/// @brief The number of castling_availability keys
constexpr std::size_t kCastlingKeys{16};

/// @brief The keys for the side to move
extern std::uint64_t kSideKey[static_cast<std::size_t>(Color::Both)];

/// @brief The keys for the en passant squares
extern std::uint64_t
    kEnPassantKey[static_cast<std::size_t>(Square::Total)];

/// @brief The keys for the castling_availability rights
extern std::uint64_t kCastleKey[kCastlingKeys];

/// @brief The keys for the pieces
extern std::uint64_t
    kPieceKey[static_cast<std::size_t>(Color::Total)]
             [static_cast<std::size_t>(Piece::Total)]
             [static_cast<std::size_t>(Square::Total)];

/// @brief Generates the hash key for a board
/// @param board The board
/// @return The hash key
std::uint64_t Hash(const Board &board) noexcept;
}   // namespace codfish::core::zobrist