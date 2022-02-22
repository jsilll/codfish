#pragma once

typedef unsigned long long U64;

constexpr U64 ONE = 1ULL;
constexpr U64 ZERO = 0ULL;
// TODO: add other handy constexprs like white/black squares ...

enum Color
{
    WHITE,
    BLACK,
    N_SIDES,
};

// clang-format off
enum Square{
    A1,B1,C1,D1,E1,F1,G1,H1,
    A2,B2,C2,D2,E2,F2,G2,H2,
    A3,B3,C3,D3,E3,F3,G3,H3,
    A4,B4,C4,D4,E4,F4,G4,H4,
    A5,B5,C5,D5,E5,F5,G5,H5,
    A6,B6,C6,D6,E6,F6,G6,H6,
    A7,B7,C7,D7,E7,F7,G7,H7,
    A8,B8,C8,D8,E8,F8,G8,H8,
    N_SQUARES,
};
// clang-format on

enum Directions
{
    NORTH = 8,
    SOUTH = -8,
    WEST = -1,
    EAST = 1,
    NORTH_WEST = 7,
    NORTH_EAST = 9,
    SOUTH_WEST = -9,
    SOUTH_EAST = -7,
    N_DIRECTIONS = 8,
};

constexpr int CASTLE_KING_WHITE = 1;
constexpr int CASTLE_QUEEN_WHITE = 2;
constexpr int CASTLE_KING_BLACK = 4;
constexpr int CASTLE_QUEEN_BLACK = 8;

enum Piece
{
    WHITE_KING,
    WHITE_QUEEN,
    WHITE_ROOK,
    WHITE_KNIGHT,
    WHITE_BISHOP,
    WHITE_PAWN,
    BLACK_KING,
    BLACK_QUEEN,
    BLACK_ROOK,
    BLACK_KNIGHT,
    BLACK_BISHOP,
    BLACK_PAWN,
    EMPTY
};

class Board; // TODO: should this be here?