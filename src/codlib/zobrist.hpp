#pragma once

#include <codlib/base.hpp>
#include <codlib/bitboard.hpp>
#include <codlib/board.hpp>

namespace zobrist {

    /// @brief Initializes the zobrist keys
    void init() noexcept;

    /// @brief The number of castling_availability keys
    constexpr std::size_t N_CASTLING_KEYS = 16;

    /// @brief The keys for the side to move
    extern bitboard::Bitboard side_key[BOTH];

    /// @brief The keys for the en passant squares
    extern bitboard::Bitboard en_passant_keys[N_SQUARES];

    /// @brief The keys for the castling_availability rights
    extern bitboard::Bitboard castle_keys[N_CASTLING_KEYS];

    /// @brief The keys for the pieces
    extern bitboard::Bitboard piece_keys[N_COLORS][N_PIECES][N_SQUARES];

    /// @brief Generates the hash key for a board
    /// @param board The board
    /// @return The hash key
    std::uint64_t generate_hash_key(const Board &board) noexcept;

}// namespace zobrist
