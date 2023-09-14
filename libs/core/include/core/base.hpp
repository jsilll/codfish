#pragma once

#include <cstdint>

namespace codfish::core {
/// @brief The ranks of the board
/// @note This enum is used both for iterating over the ranks and for indexing
/// @note into arrays
enum class Rank : std::uint8_t {
    R1 [[maybe_unused]],
    R2 [[maybe_unused]],
    R3 [[maybe_unused]],
    R4 [[maybe_unused]],
    R5 [[maybe_unused]],
    R6 [[maybe_unused]],
    R7 [[maybe_unused]],
    R8 [[maybe_unused]],
    /// @brief Used for iterating over the ranks
    Total [[maybe_unused]],
};

/// @brief The files of the board
/// @note This enum is used both for iterating over the files and for indexing
/// @note into arrays
enum class File : std::uint8_t {
    FA [[maybe_unused]],
    FB [[maybe_unused]],
    FC [[maybe_unused]],
    FD [[maybe_unused]],
    FE [[maybe_unused]],
    FF [[maybe_unused]],
    FG [[maybe_unused]],
    FH [[maybe_unused]],
    /// @brief Used for iterating over the files
    Total [[maybe_unused]],
};

/// @brief The squares of the board
/// @note This enum is used both for iterating over the squares and for indexing
/// @note into arrays
enum class Square : std::uint8_t {
    A1 [[maybe_unused]],
    B1 [[maybe_unused]],
    C1 [[maybe_unused]],
    D1 [[maybe_unused]],
    E1 [[maybe_unused]],
    F1 [[maybe_unused]],
    G1 [[maybe_unused]],
    H1 [[maybe_unused]],
    A2 [[maybe_unused]],
    B2 [[maybe_unused]],
    C2 [[maybe_unused]],
    D2 [[maybe_unused]],
    E2 [[maybe_unused]],
    F2 [[maybe_unused]],
    G2 [[maybe_unused]],
    H2 [[maybe_unused]],
    A3 [[maybe_unused]],
    B3 [[maybe_unused]],
    C3 [[maybe_unused]],
    D3 [[maybe_unused]],
    E3 [[maybe_unused]],
    F3 [[maybe_unused]],
    G3 [[maybe_unused]],
    H3 [[maybe_unused]],
    A4 [[maybe_unused]],
    B4 [[maybe_unused]],
    C4 [[maybe_unused]],
    D4 [[maybe_unused]],
    E4 [[maybe_unused]],
    F4 [[maybe_unused]],
    G4 [[maybe_unused]],
    H4 [[maybe_unused]],
    A5 [[maybe_unused]],
    B5 [[maybe_unused]],
    C5 [[maybe_unused]],
    D5 [[maybe_unused]],
    E5 [[maybe_unused]],
    F5 [[maybe_unused]],
    G5 [[maybe_unused]],
    H5 [[maybe_unused]],
    A6 [[maybe_unused]],
    B6 [[maybe_unused]],
    C6 [[maybe_unused]],
    D6 [[maybe_unused]],
    E6 [[maybe_unused]],
    F6 [[maybe_unused]],
    G6 [[maybe_unused]],
    H6 [[maybe_unused]],
    A7 [[maybe_unused]],
    B7 [[maybe_unused]],
    C7 [[maybe_unused]],
    D7 [[maybe_unused]],
    E7 [[maybe_unused]],
    F7 [[maybe_unused]],
    G7 [[maybe_unused]],
    H7 [[maybe_unused]],
    A8 [[maybe_unused]],
    B8 [[maybe_unused]],
    C8 [[maybe_unused]],
    D8 [[maybe_unused]],
    E8 [[maybe_unused]],
    F8 [[maybe_unused]],
    G8 [[maybe_unused]],
    H8 [[maybe_unused]],
    /// @brief Used for iterating over the squares
    Total [[maybe_unused]],
    /// @brief Used for representing an empty square
    Empty [[maybe_unused]] = 64,
};

/// @brief The colors of the pieces
/// @note This enum is used both for iterating over the colors and for indexing
/// @note into arrays
enum class Color : std::uint8_t {
    White [[maybe_unused]],
    Black [[maybe_unused]],
    /// @brief Used for iterating over the colors not including Both
    Total [[maybe_unused]],
    /// @brief Used for representing both colors for occupancy bitboards
    Both [[maybe_unused]] = 2,
};

/// @brief The types of pieces
/// @note This enum is used both for iterating over the piece types and for
/// @note indexing into arrays
enum class Piece : std::uint8_t {
    Pawn [[maybe_unused]],
    Knight [[maybe_unused]],
    Bishop [[maybe_unused]],
    Rook [[maybe_unused]],
    Queen [[maybe_unused]],
    King [[maybe_unused]],
    /// @brief Used for iterating over the piece types not including Empty
    Total [[maybe_unused]],
    /// @brief Used for representing an empty piece
    Empty [[maybe_unused]] = 6,
};

/// @brief The different types of castling_availability rights
/// @note The values are powers of 2 so that each bit represents a different
/// @note castling_availability right
enum class Castle : std::uint8_t {
    None [[maybe_unused]] = 0,
    WhiteKing [[maybe_unused]] = 1,
    WhiteQueen [[maybe_unused]] = 2,
    BlackKing [[maybe_unused]] = 4,
    BlackQueen [[maybe_unused]] = 8,
    All [[maybe_unused]] = 15,
};
}   // namespace codfish::core