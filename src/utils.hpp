#pragma once

#include "defs.hpp"

namespace utils
{

    inline int getSquare(const int rank, const int file)
    {
        return 8 * rank + file;
    }

    inline int getFile(const int square)
    {
        return square & 7;
    }

    inline int getRank(const int square)
    {
        return square >> 3;
    }

    inline unsigned int bitCount(U64 bitboard)
    {
        // Brian Kernighan's way
        unsigned int count = 0;
        while (bitboard)
        {
            count++;
            bitboard &= bitboard - 1;
        }
        return count;
    }

    inline void setBit(U64 &b, int sq) { b |= (1ULL << sq); }
    inline void popBit(U64 &b, int sq) { b &= ~(1ULL << sq); }
    inline bool getBit(U64 b, int sq) { return ((b >> sq) & 1ULL); }

    inline U64 soutOne(U64 b) { return (b >> 8); }
    inline U64 nortOne(U64 b) { return (b << 8); }
    inline U64 eastOne(U64 b) { return (b << 1) & 0xFEFEFEFEFEFEFEFE; }
    inline U64 noEaOne(U64 b) { return (b << 9) & 0xFEFEFEFEFEFEFEFE; }
    inline U64 soEaOne(U64 b) { return (b >> 7) & 0xFEFEFEFEFEFEFEFE; }
    inline U64 westOne(U64 b) { return (b >> 1) & 0x7F7F7F7F7F7F7F7F; }
    inline U64 soWeOne(U64 b) { return (b >> 9) & 0x7F7F7F7F7F7F7F7F; }
    inline U64 noWeOne(U64 b) { return (b << 7) & 0x7F7F7F7F7F7F7F7F; }

    int bitScan(U64 bitmap);

    void printBB(U64 bb);

} // namespace utils