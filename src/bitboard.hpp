#pragma once

#include "tables.hpp"

typedef unsigned long long U64;

namespace bb
{

    enum Color
    {
        WHITE,
        BLACK,
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
        N_DIRECTIONS = 8
    };

    constexpr U64 ONE = 1ULL;
    constexpr U64 ZERO = 0ULL;

    inline int get_square_index(const int rank_index, const int file_index)
    {
        return 8 * rank_index + file_index;
    }
    inline int get_file_index(const int square_index)
    {
        return square_index & 7;
    }
    inline int get_square_index(const int square_index)
    {
        return square_index >> 3;
    }

    inline U64 soutOne(U64 b) { return (b >> 8); }
    inline U64 nortOne(U64 b) { return (b << 8); }
    inline U64 eastOne(U64 b) { return (b << 1) & tables::CLEAR_FILE[0]; }
    inline U64 noEaOne(U64 b) { return (b << 9) & tables::CLEAR_FILE[0]; }
    inline U64 soEaOne(U64 b) { return (b >> 7) & tables::CLEAR_FILE[0]; }
    inline U64 westOne(U64 b) { return (b >> 1) & tables::CLEAR_FILE[7]; }
    inline U64 soWeOne(U64 b) { return (b >> 9) & tables::CLEAR_FILE[7]; }
    inline U64 noWeOne(U64 b) { return (b << 7) & tables::CLEAR_FILE[7]; }

    void print_bb(U64 bb);
}

// bb:Square operator overrides

inline U64 operator~(bb::Square sq)
{
    return ~tables::SQUARE_BB[sq];
}

// U64 <-> bb::Square operator overrides

inline U64 operator&(const U64 b, const bb::Square sq)
{
    return b & tables::SQUARE_BB[sq];
}

inline U64 operator|(const U64 b, const bb::Square sq)
{
    return b | tables::SQUARE_BB[sq];
}

inline U64 operator^(const U64 b, const bb::Square sq)
{
    return b ^ tables::SQUARE_BB[sq];
}

inline U64 &operator&=(U64 &b, const bb::Square sq)
{
    return b &= tables::SQUARE_BB[sq];
}

inline U64 &operator|=(U64 &b, const bb::Square sq)
{
    return b |= tables::SQUARE_BB[sq];
}

inline U64 &operator^=(U64 &b, const bb::Square sq)
{
    return b ^= tables::SQUARE_BB[sq];
}