#pragma once

#include <string>

/// @brief The ranks of the board
enum Rank : int
{
  RANK_1,
  RANK_2,
  RANK_3,
  RANK_4,
  RANK_5,
  RANK_6,
  RANK_7,
  RANK_8,
  /// @brief Used for iterating over the ranks
  N_RANKS,
};

/// @brief The files of the board
enum File : int
{
  FILE_A,
  FILE_B,
  FILE_C,
  FILE_D,
  FILE_E,
  FILE_F,
  FILE_G,
  FILE_H,
  /// @brief Used for iterating over the files
  N_FILES,
};

// clang-format off
/// @brief The squares of the board
enum Square : int {
  A1, B1, C1, D1, E1, F1, G1, H1,
  A2, B2, C2, D2, E2, F2, G2, H2,
  A3, B3, C3, D3, E3, F3, G3, H3,
  A4, B4, C4, D4, E4, F4, G4, H4,
  A5, B5, C5, D5, E5, F5, G5, H5,
  A6, B6, C6, D6, E6, F6, G6, H6,
  A7, B7, C7, D7, E7, F7, G7, H7,
  A8, B8, C8, D8, E8, F8, G8, H8,
  /// @brief Used for iterating over the squares
  N_SQUARES,
  /// @brief Used for representing an empty square
  EMPTY_SQUARE = -1
};
// clang-format on

// clang-format off
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
    "-"};
// clang-format on

/// @brief The directions of the board represented as offsets
enum Direction : int
{
  NORTH = 8,
  SOUTH = -8,
  WEST = -1,
  EAST = 1,
  NORTH_WEST = 7,
  NORTH_EAST = 9,
  SOUTH_WEST = -9,
  SOUTH_EAST = -7,
  /// @brief Used for iterating over all directions
  N_DIRECTIONS = 8,
};

/// @brief The colors of the pieces
enum Color : int
{
  WHITE = 0,
  BLACK = 1,
  BOTH = 2,
  /// @brief Used for iterating over the colors not including BOTH
  N_SIDES = 2,
};

/// @brief The types of pieces
enum PieceType : int
{
  PAWN = 0,
  KNIGHT = 1,
  BISHOP = 2,
  ROOK = 3,
  QUEEN = 4,
  KING = 5,
  /// @brief Used for iterating over the piece types not including EMPTY_PIECE
  N_PIECES = 6,
  /// @brief Used for representing an empty piece
  EMPTY_PIECE = 6,
};

// clang-format off
/// @brief The representation of the pieces aligned with the PieceType enum
/// @note The first 6 are the ascii representation, the next 6 are unicode
const std::string PIECE_REPR[26] = {
    "P", "N", "B", "R", "Q", "K",
    "p", "n", "b", "r", "q", "k",
    " ",
    "♙", "♘", "♗", "♖", "♕", "♔",
    "♟︎", "♞", "♝", "♜", "♛", "♚",
    " ",
};
// clang-format on

/// @brief The different types of castling rights
/// @note The values are powers of 2 so that they can be combined with bitwise operators
enum CastlingRight : int
{
  CASTLE_KING_WHITE = 1,
  CASTLE_QUEEN_WHITE = 2,
  CASTLE_KING_BLACK = 4,
  CASTLE_QUEEN_BLACK = 8
};