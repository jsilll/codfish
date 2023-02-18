#pragma once

#include <string>

/// @brief The ranks of the board
/// @note This enum is used both for iterating over the ranks and for indexing into arrays
enum Rank : int {
    RANK_1 [[maybe_unused]],
    RANK_2 [[maybe_unused]],
    RANK_3 [[maybe_unused]],
    RANK_4 [[maybe_unused]],
    RANK_5 [[maybe_unused]],
    RANK_6 [[maybe_unused]],
    RANK_7 [[maybe_unused]],
    RANK_8 [[maybe_unused]],
    /// @brief Used for iterating over the ranks
    N_RANKS [[maybe_unused]],
};

/// @brief The files of the board
/// @note This enum is used both for iterating over the files and for indexing into arrays
enum File : int {
    FILE_A [[maybe_unused]],
    FILE_B [[maybe_unused]],
    FILE_C [[maybe_unused]],
    FILE_D [[maybe_unused]],
    FILE_E [[maybe_unused]],
    FILE_F [[maybe_unused]],
    FILE_G [[maybe_unused]],
    FILE_H [[maybe_unused]],
    /// @brief Used for iterating over the files
    N_FILES [[maybe_unused]],
};

/// @brief The squares of the board
/// @note This enum is used both for iterating over the squares and for indexing into arrays
enum Square : int {
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
    N_SQUARES [[maybe_unused]],
    /// @brief Used for representing an empty square
    EMPTY_SQUARE [[maybe_unused]] = 64,
};

/// @brief The colors of the pieces
/// @note This enum is used both for iterating over the colors and for indexing into arrays
enum Color : int {
    WHITE [[maybe_unused]], BLACK [[maybe_unused]], /// @brief Used for iterating over the colors not including BOTH
    N_COLORS [[maybe_unused]], /// @brief Used for representing both colors for occupancy bitboards
    BOTH [[maybe_unused]] = 2,
};

/// @brief The types of pieces
/// @note This enum is used both for iterating over the piece types and for indexing into arrays
enum PieceType : int {
    PAWN [[maybe_unused]],
    KNIGHT [[maybe_unused]],
    BISHOP [[maybe_unused]],
    ROOK [[maybe_unused]],
    QUEEN [[maybe_unused]],
    KING [[maybe_unused]],
    /// @brief Used for iterating over the piece types not including EMPTY_PIECE
    N_PIECES [[maybe_unused]],
    /// @brief Used for representing an empty piece
    EMPTY_PIECE [[maybe_unused]] = 6,
};

/// @brief The different types of castling_availability rights
/// @note The values are powers of 2 so that each bit represents a different castling_availability right: 0001 = WHITE_KING, 0010 = WHITE_QUEEN, 0100 = BLACK_KING, 1000 = BLACK_QUEEN
enum CastlingAvailability : int {
    NONE [[maybe_unused]] = 0,
    WHITE_KING [[maybe_unused]] = 1,
    WHITE_QUEEN [[maybe_unused]] = 2,
    BLACK_KING [[maybe_unused]] = 4,
    BLACK_QUEEN [[maybe_unused]] = 8,
    ALL [[maybe_unused]] = 15,
};