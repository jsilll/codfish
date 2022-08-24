#pragma once

#include <cstdint>

#include <engine/constants.hpp>

typedef std::uint64_t u64;

constexpr u64 ONE = 1ULL;
constexpr u64 ZERO = 0ULL;

namespace bitboard
{

    // Common Bitboard Operations
    inline bool get_bit(u64 bb, Square sq) { return ((bb >> sq) & 1ULL); }
    inline void pop_bit(u64 &bn, Square sq) { bn &= ~(1ULL << sq); }
    inline void pop_last_bit(u64 &bb) { bb &= bb - 1; }
    inline void set_bit(u64 &bb, Square sq) { bb |= (1ULL << sq); }

    // Common Bitboard Shifts
    inline u64 sout_one(u64 bb) { return (bb >> 8); }
    inline u64 nort_one(u64 bb) { return (bb << 8); }
    inline u64 east_one(u64 bb) { return (bb << 1) & 0xFEFEFEFEFEFEFEFE; }
    inline u64 no_ea_one(u64 bb) { return (bb << 9) & 0xFEFEFEFEFEFEFEFE; }
    inline u64 so_ea_one(u64 bb) { return (bb >> 7) & 0xFEFEFEFEFEFEFEFE; }
    inline u64 west_one(u64 bb) { return (bb >> 1) & 0x7F7F7F7F7F7F7F7F; }
    inline u64 so_we_one(u64 bb) { return (bb >> 9) & 0x7F7F7F7F7F7F7F7F; }
    inline u64 no_we_one(u64 bb) { return (bb << 7) & 0x7F7F7F7F7F7F7F7F; }

    /**
     * @brief Returns number of bits in Bitboard
     *
     * @param bb
     * @return unsigned int
     */
    int bit_count(u64 bb);

    /**
     * @brief Returns index of LSB bit
     *
     * @param bb
     * @return unsigned int
     */
    Square bit_scan(u64 bb);

    /**
     * @brief Returns index of LSB bit
     *
     * @param bb
     * @return unsigned int
     */
    Square bit_scan_forward(u64 bb);

    /**
     * @brief Sets the occupancy bits
     *
     * @param index
     * @param bits_in_mask
     * @param attack_mask
     * @return U64
     */
    u64 set_occupancy(int index, int bits_in_mask, u64 attack_mask);

    /**
     * @brief Prints a bitboard
     *
     * @param bb
     */
    void print(u64 bb);

} // namespace bitboard
