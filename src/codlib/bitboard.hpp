#pragma once

#include <cstdint>

#include "codlib/base.hpp"

/// @brief Bitboard type
typedef std::uint64_t u64;

namespace bitboard
{
    /// @brief Bitboard constant for bit manipulation
    constexpr u64 kONE{1};

    /// @brief Bitboard constant for bit manipulation
    constexpr u64 kZERO{0};

    /// @brief Pops the last bit in a bitboard
    /// @param bb The bitboard
    constexpr void pop_last_bit(u64 &bb) noexcept { bb &= bb - kONE; }

    /// @brief Sets a bit in a bitboard
    /// @param bb The bitboard
    /// @param sq The square
    constexpr void set_bit(u64 &bb, Square sq) noexcept { bb |= (kONE << sq); }

    /// @brief Pops a bit in a bitboard
    /// @param bn The bitboard
    /// @param sq The square
    constexpr void pop_bit(u64 &bb, Square sq) noexcept { bb &= ~(kONE << sq); }

    /// @brief Gets a bit in a bitboard
    /// @param bb The bitboard
    /// @param sq The square
    /// @return Whether the bit is set or not
    [[nodiscard]] constexpr bool get_bit(u64 bb, Square sq) noexcept { return ((bb >> sq) & kONE); }

    /// @brief Shifts a bitboard one square south
    /// @param bb The bitboard
    /// @return The shifted bitboard
    [[nodiscard]] constexpr u64 sout_one(u64 bb) noexcept { return (bb >> 8); }

    /// @brief Shifts a bitboard one square north
    /// @param bb The bitboard
    /// @return The shifted bitboard
    [[nodiscard]] constexpr u64 nort_one(u64 bb) noexcept { return (bb << 8); }

    /// @brief Shifts a bitboard one square east
    /// @param bb The bitboard
    /// @return The shifted bitboard
    [[nodiscard]] constexpr u64 east_one(u64 bb) noexcept { return (bb << 1) & 0xFEFEFEFEFEFEFEFE; }

    /// @brief Shifts a bitboard one square north east
    /// @param bb The bitboard
    /// @return The shifted bitboard
    [[nodiscard]] constexpr u64 no_ea_one(u64 bb) noexcept { return (bb << 9) & 0xFEFEFEFEFEFEFEFE; }

    /// @brief Shifts a bitboard one square south east
    /// @param bb The bitboard
    /// @return The shifted bitboard
    [[nodiscard]] constexpr u64 so_ea_one(u64 bb) noexcept { return (bb >> 7) & 0xFEFEFEFEFEFEFEFE; }

    /// @brief Shifts a bitboard one square west
    /// @param bb The bitboard
    /// @return The shifted bitboard
    [[nodiscard]] constexpr u64 west_one(u64 bb) noexcept { return (bb >> 1) & 0x7F7F7F7F7F7F7F7F; }

    /// @brief Shifts a bitboard one square south west
    /// @param bb The bitboard
    /// @return The shifted bitboard
    [[nodiscard]] constexpr u64 so_we_one(u64 bb) noexcept { return (bb >> 9) & 0x7F7F7F7F7F7F7F7F; }

    /// @brief Shifts a bitboard one square north west
    /// @param bb The bitboard
    /// @return The shifted bitboard
    [[nodiscard]] constexpr u64 no_we_one(u64 bb) noexcept { return (bb << 7) & 0x7F7F7F7F7F7F7F7F; }

    /// @brief Returns the number of bits in a bitboard
    /// @param bb The bitboard
    /// @return The number of bits in the bitboard
    [[nodiscard]] constexpr int bit_count(u64 bb) noexcept
    {
        unsigned int count = 0;
        while (bb)
        {
            count++;
            bb &= bb - 1;
        }
        return static_cast<int>(count);
    }

    /// @brief Returns index of LSB bit
    /// @param bb The bitboard
    /// @return The index of the LSB bit
    [[nodiscard]] constexpr Square bit_scan(u64 bb) noexcept
    {
        if (bb)
        {
            return (Square)bit_count((bb & -bb) - 1);
        }

        return EMPTY_SQUARE;
    }

    /// @brief Returns index of LSB bit
    /// @param bb The bitboard
    /// @return The index of the LSB bit
    [[nodiscard]] constexpr Square bit_scan_forward(u64 bb) noexcept
    {
        // clang-format off
        const int index64[64] = {
            0, 47, 1, 56, 48, 27, 2, 60,
            57, 49, 41, 37, 28, 16, 3, 61,
            54, 58, 35, 52, 50, 42, 21, 44,
            38, 32, 29, 23, 17, 11, 4, 62,
            46, 55, 26, 59, 40, 36, 15, 53,
            34, 51, 20, 43, 31, 22, 10, 45,
            25, 39, 14, 33, 19, 30, 9, 24,
            13, 18, 8, 12, 7, 6, 5, 63};
        // clang-format on

        const u64 debruijn64 = 0x03f79d71b4cb0a89;
        return (Square)index64[((bb ^ (bb - 1)) * debruijn64) >> 58];
    }

    /// @brief Sets the occupancy bits
    /// @param index Index of the bitboard
    /// @param bits_in_mask Number of bits in the mask
    /// @param attack_mask The attack mask
    /// @return The occupancy bits
    [[nodiscard]] u64 set_occupancy(int index, int bits_in_mask, u64 attack_mask) noexcept;

    /// @brief Prints a bitboard to the an output stream
    /// @param os The output stream 
    /// @param bb The bitboard 
    /// @return The output stream 
    std::ostream &operator<<(std::ostream &os, const u64 &bb) noexcept;
} // namespace bitboard
