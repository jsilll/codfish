#pragma once

#include "codlib/base.hpp"
#include "codlib/bitboard.hpp"
#include "utils.hpp"

#include <string>
#include <vector>

namespace utils {
    /// @brief Bitboard masks for each rank
    constexpr bitboard::u64 MASK_RANK[] = {0xFF,
                                           0xFF00,
                                           0xFF0000,
                                           0xFF000000,
                                           0xFF00000000,
                                           0xFF0000000000,
                                           0xFF000000000000,
                                           0xFF00000000000000};

    /// @brief Bitboard masks for each file
    constexpr bitboard::u64 MASK_FILE[] = {0x0101010101010101,
                                           0x202020202020202,
                                           0x404040404040404,
                                           0x808080808080808,
                                           0x1010101010101010,
                                           0x2020202020202020,
                                           0x4040404040404040,
                                           0x8080808080808080};

    /// @brief Bitboard masks for clearing each rank
    constexpr bitboard::u64 MASK_CLEAR_RANK[] = {0xFFFFFFFFFFFFFF00,
                                                 0xFFFFFFFFFFFF00FF,
                                                 0xFFFFFFFFFF00FFFF,
                                                 0xFFFFFFFF00FFFFFF, 0xFFFFFF00FFFFFFFF,
                                                 0xFFFF00FFFFFFFFFF,
                                                 0xFF00FFFFFFFFFFFF,
                                                 0x00FFFFFFFFFFFFFF};

    /// @brief Bitboard masks for clearing each file
    constexpr bitboard::u64 MASK_CLEAR_FILE[] = {0xFEFEFEFEFEFEFEFE,
                                                 0xFDFDFDFDFDFDFDFD,
                                                 0xFBFBFBFBFBFBFBFB,
                                                 0xF7F7F7F7F7F7F7F7,
                                                 0xEFEFEFEFEFEFEFEF,
                                                 0xDFDFDFDFDFDFDFDF,
                                                 0xBFBFBFBFBFBFBFBF,
                                                 0x7F7F7F7F7F7F7F7F};

    /// @brief Bitboard representation of each square
    constexpr bitboard::u64 SQUARE_BB[N_SQUARES] = {
            0x1ULL, 0x2ULL, 0x4ULL, 0x8ULL, 0x10ULL, 0x20ULL, 0x40ULL, 0x80ULL,
            0x100ULL, 0x200ULL, 0x400ULL, 0x800ULL, 0x1000ULL, 0x2000ULL, 0x4000ULL, 0x8000ULL,
            0x10000ULL, 0x20000ULL, 0x40000ULL, 0x80000ULL, 0x100000ULL, 0x200000ULL, 0x400000ULL, 0x800000ULL,
            0x1000000ULL, 0x2000000ULL, 0x4000000ULL, 0x8000000ULL, 0x10000000ULL, 0x20000000ULL, 0x40000000ULL,
            0x80000000ULL,
            0x100000000ULL, 0x200000000ULL, 0x400000000ULL, 0x800000000ULL, 0x1000000000ULL, 0x2000000000ULL,
            0x4000000000ULL, 0x8000000000ULL,
            0x10000000000ULL, 0x20000000000ULL, 0x40000000000ULL, 0x80000000000ULL, 0x100000000000ULL,
            0x200000000000ULL, 0x400000000000ULL, 0x800000000000ULL,
            0x1000000000000ULL, 0x2000000000000ULL, 0x4000000000000ULL, 0x8000000000000ULL, 0x10000000000000ULL,
            0x20000000000000ULL, 0x40000000000000ULL, 0x80000000000000ULL,
            0x100000000000000ULL, 0x200000000000000ULL, 0x400000000000000ULL, 0x800000000000000ULL,
            0x1000000000000000ULL, 0x2000000000000000ULL, 0x4000000000000000ULL, 0x8000000000000000ULL,
    };

    /// @brief Relevant bits count for each square for the bishop
    constexpr int RELEVANT_BITS_COUNT_BISHOP[N_SQUARES] = {
            6, 5, 5, 5, 5, 5, 5, 6,
            5, 5, 5, 5, 5, 5, 5, 5,
            5, 5, 7, 7, 7, 7, 5, 5,
            5, 5, 7, 9, 9, 7, 5, 5,
            5, 5, 7, 9, 9, 7, 5, 5,
            5, 5, 7, 7, 7, 7, 5, 5,
            5, 5, 5, 5, 5, 5, 5, 5,
            6, 5, 5, 5, 5, 5, 5, 6,
    };

    /// @brief Relevant bits count for each square for the rook
    constexpr int RELEVANT_BITS_COUNT_ROOK[N_SQUARES] = {
            12, 11, 11, 11, 11, 11, 11, 12,
            11, 10, 10, 10, 10, 10, 10, 11,
            11, 10, 10, 10, 10, 10, 10, 11,
            11, 10, 10, 10, 10, 10, 10, 11,
            11, 10, 10, 10, 10, 10, 10, 11,
            11, 10, 10, 10, 10, 10, 10, 11,
            11, 10, 10, 10, 10, 10, 10, 11,
            12, 11, 11, 11, 11, 11, 11, 12,
    };

/// @brief Returns the file of a square
/// @param sq The square
/// @return The file
    [[nodiscard]] constexpr File get_file(const Square sq) noexcept { return static_cast<File>(sq & 7); }

/// @brief Returns the rank of a square
/// @param sq The square
/// @return The rank
    [[nodiscard]] constexpr Rank get_rank(const Square sq) noexcept { return static_cast<Rank>(sq >> 3); }

/// @brief Returns the inactive color
/// @param to_move The color to move
/// @return The inactive color
    [[nodiscard]] constexpr Color get_opponent(const Color to_move) noexcept {
        return static_cast<Color>((int) to_move ^ 1);
    }

/// @brief Returns the square of a rank and file
/// @param rk The rank
/// @param fl The file
/// @return The square
    [[nodiscard]] constexpr Square get_square(const Rank rk, const File fl) noexcept {
        return static_cast<Square>(8 * rk + fl);
    }
}// namespace utils