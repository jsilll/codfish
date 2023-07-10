#pragma once

#include <string>
#include <vector>

#include <codchess/base.hpp>
#include <codchess/bitboard.hpp>

namespace cod::chess::utils {
/// @brief Bitboard masks for each rank
static constexpr bitboard::Bitboard kMaskRank[] = {
    0xFF,         0xFF00,         0xFF0000,         0xFF000000,
    0xFF00000000, 0xFF0000000000, 0xFF000000000000, 0xFF00000000000000};

/// @brief Bitboard masks for each file
[[maybe_unused]] static constexpr bitboard::Bitboard kMaskFile[] = {
    0x0101010101010101, 0x202020202020202,  0x404040404040404,
    0x808080808080808,  0x1010101010101010, 0x2020202020202020,
    0x4040404040404040, 0x8080808080808080};

/// @brief Bitboard masks for clearing each rank
static constexpr bitboard::Bitboard kMaskNotRank[] = {
    0xFFFFFFFFFFFFFF00, 0xFFFFFFFFFFFF00FF, 0xFFFFFFFFFF00FFFF,
    0xFFFFFFFF00FFFFFF, 0xFFFFFF00FFFFFFFF, 0xFFFF00FFFFFFFFFF,
    0xFF00FFFFFFFFFFFF, 0x00FFFFFFFFFFFFFF};

/// @brief Bitboard masks for clearing each file
static constexpr bitboard::Bitboard kMaskNotFile[] = {
    0xFEFEFEFEFEFEFEFE, 0xFDFDFDFDFDFDFDFD, 0xFBFBFBFBFBFBFBFB,
    0xF7F7F7F7F7F7F7F7, 0xEFEFEFEFEFEFEFEF, 0xDFDFDFDFDFDFDFDF,
    0xBFBFBFBFBFBFBFBF, 0x7F7F7F7F7F7F7F7F};

/// @brief Bitboard representation of each square
static constexpr bitboard::Bitboard
    kSquareToBitboard[static_cast<std::size_t>(Square::Total)] = {
        0x1ULL,
        0x2ULL,
        0x4ULL,
        0x8ULL,
        0x10ULL,
        0x20ULL,
        0x40ULL,
        0x80ULL,
        0x100ULL,
        0x200ULL,
        0x400ULL,
        0x800ULL,
        0x1000ULL,
        0x2000ULL,
        0x4000ULL,
        0x8000ULL,
        0x10000ULL,
        0x20000ULL,
        0x40000ULL,
        0x80000ULL,
        0x100000ULL,
        0x200000ULL,
        0x400000ULL,
        0x800000ULL,
        0x1000000ULL,
        0x2000000ULL,
        0x4000000ULL,
        0x8000000ULL,
        0x10000000ULL,
        0x20000000ULL,
        0x40000000ULL,
        0x80000000ULL,
        0x100000000ULL,
        0x200000000ULL,
        0x400000000ULL,
        0x800000000ULL,
        0x1000000000ULL,
        0x2000000000ULL,
        0x4000000000ULL,
        0x8000000000ULL,
        0x10000000000ULL,
        0x20000000000ULL,
        0x40000000000ULL,
        0x80000000000ULL,
        0x100000000000ULL,
        0x200000000000ULL,
        0x400000000000ULL,
        0x800000000000ULL,
        0x1000000000000ULL,
        0x2000000000000ULL,
        0x4000000000000ULL,
        0x8000000000000ULL,
        0x10000000000000ULL,
        0x20000000000000ULL,
        0x40000000000000ULL,
        0x80000000000000ULL,
        0x100000000000000ULL,
        0x200000000000000ULL,
        0x400000000000000ULL,
        0x800000000000000ULL,
        0x1000000000000000ULL,
        0x2000000000000000ULL,
        0x4000000000000000ULL,
        0x8000000000000000ULL,
};

/// @brief Relevant bits count for each square for the bishop
static constexpr int
    kBishopRelevantBits[static_cast<std::size_t>(Square::Total)] = {
        6, 5, 5, 5, 5, 5, 5, 6, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 7, 7, 7, 7,
        5, 5, 5, 5, 7, 9, 9, 7, 5, 5, 5, 5, 7, 9, 9, 7, 5, 5, 5, 5, 7, 7,
        7, 7, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 6, 5, 5, 5, 5, 5, 5, 6,
};

/// @brief Relevant bits count for each square for the rook
static constexpr int
    kRookRelevantBits[static_cast<std::size_t>(Square::Total)] = {
        12, 11, 11, 11, 11, 11, 11, 12, 11, 10, 10, 10, 10, 10, 10, 11,
        11, 10, 10, 10, 10, 10, 10, 11, 11, 10, 10, 10, 10, 10, 10, 11,
        11, 10, 10, 10, 10, 10, 10, 11, 11, 10, 10, 10, 10, 10, 10, 11,
        11, 10, 10, 10, 10, 10, 10, 11, 12, 11, 11, 11, 11, 11, 11, 12,
};

/// @brief Returns the file of a square
/// @param sq The square
/// @return The file
[[nodiscard]] constexpr File
GetFile(const Square sq) noexcept {
    return static_cast<File>(static_cast<std::uint8_t>(sq) & 7);
}

/// @brief Returns the rank of a square
/// @param sq The square
/// @return The rank
[[nodiscard]] constexpr Rank
GetRank(const Square sq) noexcept {
    return static_cast<Rank>(static_cast<std::uint8_t>(sq) >> 3);
}

/// @brief Returns the square of a rank and file
/// @param rk The rank
/// @param fl The file
/// @return The square
[[nodiscard]] constexpr Square
GetSquare(const Rank rk, const File fl) noexcept {
    return static_cast<Square>(8 * static_cast<std::uint8_t>(rk) +
                               static_cast<std::uint8_t>(fl));
}

/// @brief Returns the square flipped vertically
/// @param sq The square
/// @return The flipped square
[[nodiscard]] constexpr Square
FlipSquare(const Square sq) {
    return static_cast<Square>(static_cast<std::uint8_t>(sq) ^ 56);
}

/// @brief Returns the opposite color
[[nodiscard]] constexpr Color
GetOpponent(const Color to_move) noexcept {
    return static_cast<Color>(static_cast<int>(to_move) ^ 1);
}

/// @brief Returns the string representation of a square
/// @param sq The square
/// @return The string representation
[[nodiscard]] std::string SquareToString(Square sq) noexcept;

/// @brief Returns the string representation of a piece
/// @param piece The piece
/// @param color The color
/// @param ascii Whether to use ascii or unicode characters
/// @return The string representation
[[nodiscard]] std::string PieceToString(Piece piece, Color color,
                                        bool ascii = true) noexcept;

/// @brief Returns whether the FEN is valid
/// @param args The FEN split by spaces
/// @return Whether the FEN is valid
[[nodiscard]] bool ValidFen(const std::string &fen) noexcept;
}   // namespace cod::chess::utils