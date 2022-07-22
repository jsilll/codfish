#pragma once

#include <engine/constants.hpp>

namespace bitboard
{
    // Common Bitboard Operations
    inline bool get_bit(U64 bb, int sq) { return ((bb >> sq) & 1ULL); }
    inline void pop_bit(U64 &bn, int sq) { bn &= ~(1ULL << sq); }
    inline void pop_last_bit(U64 &bb) { bb &= bb - 1; }
    inline void set_bit(U64 &bb, int sq) { bb |= (1ULL << sq); }

    // Common Bitboard Shifts
    inline U64 sout_one(U64 bb) { return (bb >> 8); }
    inline U64 nort_one(U64 bb) { return (bb << 8); }
    inline U64 east_one(U64 bb) { return (bb << 1) & 0xFEFEFEFEFEFEFEFE; }
    inline U64 no_ea_one(U64 bb) { return (bb << 9) & 0xFEFEFEFEFEFEFEFE; }
    inline U64 so_ea_one(U64 bb) { return (bb >> 7) & 0xFEFEFEFEFEFEFEFE; }
    inline U64 west_one(U64 bb) { return (bb >> 1) & 0x7F7F7F7F7F7F7F7F; }
    inline U64 so_we_one(U64 bb) { return (bb >> 9) & 0x7F7F7F7F7F7F7F7F; }
    inline U64 no_we_one(U64 bb) { return (bb << 7) & 0x7F7F7F7F7F7F7F7F; }

    /**
     * @brief Returns number of bits in Bitboard
     *
     * @param bb
     * @return unsigned int
     */
    int bit_count(U64 bb);

    /**
     * @brief Returns index of LSB bit
     *
     * @param bb
     * @return unsigned int
     */
    int bit_scan(U64 bb);

    /**
     * @brief Returns index of LSB bit
     *
     * @param bb
     * @return unsigned int
     */
    int bit_scan_forward(U64 bb);

    /**
     * @brief Sets the occupancy bits
     *
     * @param index
     * @param bits_in_mask
     * @param attack_mask
     * @return U64
     */
    U64 set_occupancy(int index, int bits_in_mask, U64 attack_mask);

    /**
     * @brief Prints a bitboard
     *
     * @param bb
     */
    void print(U64 bb);

} // namespace bitboard
