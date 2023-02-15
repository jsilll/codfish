#pragma once

#include <string>

/// @brief The ranks of the board
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
    EMPTY_SQUARE [[maybe_unused]] = -1
};

/// @brief The names of the squares aligned with the Square enum
const std::string SQUARE_NAMES[] = {
        "a1", "b1", "c1", "d1", "e1", "f1", "g1", "h1",
        "a2", "b2", "c2", "d2", "e2", "f2", "g2", "h2",
        "a3", "b3", "c3", "d3", "e3", "f3", "g3", "h3",
        "a4", "b4", "c4", "d4", "e4", "f4", "g4", "h4",
        "a5", "b5", "c5", "d5", "e5", "f5", "g5", "h5",
        "a6", "b6", "c6", "d6", "e6", "f6", "g6", "h6",
        "a7", "b7", "c7", "d7", "e7", "f7", "g7", "h7",
        "a8", "b8", "c8", "d8", "e8", "f8", "g8", "h8",
        "-"};// TODO: remove this from this file

/// @brief The directions of the board represented as offsets
enum Direction : int {
    NORTH [[maybe_unused]] = 8,
    SOUTH [[maybe_unused]] = -8,
    WEST [[maybe_unused]] = -1,
    EAST [[maybe_unused]] = 1,
    NORTH_WEST [[maybe_unused]] = 7,
    NORTH_EAST [[maybe_unused]] = 9,
    SOUTH_WEST [[maybe_unused]] = -9,
    SOUTH_EAST [[maybe_unused]] = -7,
    /// @brief Used for iterating over all directions
    N_DIRECTIONS [[maybe_unused]] = 8,
};// TODO: remove this from this file

/// @brief The colors of the pieces
enum Color : int {
    WHITE [[maybe_unused]] = 0,
    BLACK [[maybe_unused]] = 1,
    BOTH [[maybe_unused]] = 2,
    /// @brief Used for iterating over the colors not including BOTH
    N_SIDES [[maybe_unused]] = 2,
};

/// @brief The types of pieces
enum PieceType : int {
    PAWN [[maybe_unused]] = 0,
    KNIGHT [[maybe_unused]] = 1,
    BISHOP [[maybe_unused]] = 2,
    ROOK [[maybe_unused]] = 3,
    QUEEN [[maybe_unused]] = 4,
    KING [[maybe_unused]] = 5,
    /// @brief Used for iterating over the piece types not including EMPTY_PIECE
    N_PIECES [[maybe_unused]] = 6,
    /// @brief Used for representing an empty piece
    EMPTY_PIECE [[maybe_unused]] = 6,
};

/// @brief The representation of the pieces aligned with the PieceType enum
/// @note The first 6 are the ascii representation, the next 6 are unicode
const std::string PIECE_REPR[26] = {
        "P",
        "N",
        "B",
        "R",
        "Q",
        "K",
        "p",
        "n",
        "b",
        "r",
        "q",
        "k",
        " ",
        "♙",
        "♘",
        "♗",
        "♖",
        "♕",
        "♔",
        "♟︎",
        "♞",
        "♝",
        "♜",
        "♛",
        "♚",
        " ",
};// TODO: remove this from this file

/// @brief The different types of castling rights
/// @note The values are powers of 2 so that they can be combined with bitwise operators
enum CastlingRight : int {
    CASTLE_KING_WHITE [[maybe_unused]] = 1,
    CASTLE_QUEEN_WHITE [[maybe_unused]] = 2,
    CASTLE_KING_BLACK [[maybe_unused]] = 4,
    CASTLE_QUEEN_BLACK [[maybe_unused]] = 8
};