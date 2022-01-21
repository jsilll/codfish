#pragma once

#include "bitboard.hpp"
#include "tables.hpp"

typedef unsigned long long U64;

namespace utils
{

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
} // namespace utils

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