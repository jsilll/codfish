#pragma once

#include <codchess/base.hpp>
#include <codchess/bitboard.hpp>
#include <codchess/board.hpp>

namespace codchess::zobrist {

    /// @brief Initializes the zobrist keys
    void Init() noexcept;

    /// @brief The number of castling_availability keys
    constexpr std::size_t N_CASTLING_KEYS = 16;

    /// @brief The keys for the side to move
    extern bitboard::Bitboard SIDE_KEY[BOTH];

    /// @brief The keys for the en passant squares
    extern bitboard::Bitboard EN_PASSANT_KEY[N_SQUARES];

    /// @brief The keys for the castling_availability rights
    extern bitboard::Bitboard CASTLE_KEY[N_CASTLING_KEYS];

    /// @brief The keys for the pieces
    extern bitboard::Bitboard PIECE_KEY[N_COLORS][N_PIECES][N_SQUARES];

    /// @brief Generates the hash key for a board
    /// @param board The board
    /// @return The hash key
    std::uint64_t generate_hash_key(const Board &board) noexcept;

}// namespace codchess::zobrist
