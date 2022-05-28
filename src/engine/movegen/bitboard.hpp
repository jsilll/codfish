#pragma once

#include "../defs.hpp"

namespace bitboard
{
    // Common Bitboard Operations
    inline bool getBit(U64 bb, int sq) { return ((bb >> sq) & 1ULL); }
    inline void popBit(U64 &bn, int sq) { bn &= ~(1ULL << sq); }
    inline void popLastBit(U64 &bb) { bb &= bb - 1; }
    inline void setBit(U64 &bb, int sq) { bb |= (1ULL << sq); }

    // Common Bitboard Shifts
    inline U64 soutOne(U64 bb) { return (bb >> 8); }
    inline U64 nortOne(U64 bb) { return (bb << 8); }
    inline U64 eastOne(U64 bb) { return (bb << 1) & 0xFEFEFEFEFEFEFEFE; }
    inline U64 noEaOne(U64 bb) { return (bb << 9) & 0xFEFEFEFEFEFEFEFE; }
    inline U64 soEaOne(U64 bb) { return (bb >> 7) & 0xFEFEFEFEFEFEFEFE; }
    inline U64 westOne(U64 bb) { return (bb >> 1) & 0x7F7F7F7F7F7F7F7F; }
    inline U64 soWeOne(U64 bb) { return (bb >> 9) & 0x7F7F7F7F7F7F7F7F; }
    inline U64 noWeOne(U64 bb) { return (bb << 7) & 0x7F7F7F7F7F7F7F7F; }

    /**
     * @brief Returns number of bits in Bitboard
     *
     * @param bb
     * @return unsigned int
     */
    int bitCount(U64 bb);

    /**
     * @brief Returns index of LSB bit
     *
     * @param bb
     * @return unsigned int
     */
    int bitScan(U64 bb);

    /**
     * @brief Returns index of LSB bit
     *
     * @param bb
     * @return unsigned int
     */
    int bitScanForward(U64 bb);

    /**
     * @brief Sets the occupancy bits
     *
     * @param index
     * @param bits_in_mask
     * @param attack_mask
     * @return U64
     */
    U64 setOccupancy(int index, int bits_in_mask, U64 attack_mask);

    /**
     * @brief Prints a bitboard
     * TODO: make this use std::cout
     *
     * @param bb
     */
    void printBB(U64 bb);
} // namespace bitboard
