#pragma once

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

    inline int get_rank_index(const int square_index)
    {
        return square_index >> 3;
    }

    inline U64 soutOne(U64 b) { return (b >> 8); }
    inline U64 nortOne(U64 b) { return (b << 8); }
    inline U64 eastOne(U64 b) { return (b << 1) & 0xFEFEFEFEFEFEFEFE; }
    inline U64 noEaOne(U64 b) { return (b << 9) & 0xFEFEFEFEFEFEFEFE; }
    inline U64 soEaOne(U64 b) { return (b >> 7) & 0xFEFEFEFEFEFEFEFE; }
    inline U64 westOne(U64 b) { return (b >> 1) & 0x7F7F7F7F7F7F7F7F; }
    inline U64 soWeOne(U64 b) { return (b >> 9) & 0x7F7F7F7F7F7F7F7F; }
    inline U64 noWeOne(U64 b) { return (b << 7) & 0x7F7F7F7F7F7F7F7F; }

    unsigned int bitCnt(U64 bitmap);
    unsigned int firstOne(U64 bitmap);
    unsigned int lastOne(U64 bitmap);

    void print_bb(U64 bb);

} // namespace utils