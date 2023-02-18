#pragma once

#include <cstdint>

#include <codlib/base.hpp>

namespace bitboard {
    // -- Constants --

    /// @brief Bitboard type
    typedef std::uint64_t Bitboard;

    /// @brief Bitboard constant for bit manipulation
    constexpr Bitboard ONE{1};

    /// @brief Bitboard constant for bit manipulation
    constexpr Bitboard ZERO{0};

    // -- Bitboard manipulation functions --

    /// @brief Pops the last bit in a bitboard
    /// @param bb The bitboard
    constexpr void pop_last_bit(Bitboard &bb) noexcept { bb &= bb - ONE; }

    /// @brief Sets a bit in a bitboard
    /// @param bb The bitboard
    /// @param sq The square
    constexpr void set_bit(Bitboard &bb, Square sq) noexcept { bb |= (ONE << sq); }

    /// @brief Pops a bit in a bitboard
    /// @param bn The bitboard
    /// @param sq The square
    constexpr void pop_bit(Bitboard &bb, Square sq) noexcept { bb &= ~(ONE << sq); }

    /// @brief Gets a bit in a bitboard
    /// @param bb The bitboard
    /// @param sq The square
    /// @return Whether the bit is set or not
    [[nodiscard]] constexpr bool get_bit(const Bitboard bb, const Square sq) noexcept { return ((bb >> sq) & ONE); }

    /// @brief Returns the number of bits in a bitboard
    /// @param bb The bitboard
    /// @return The number of bits in the bitboard
    [[nodiscard]] constexpr int bit_count(Bitboard bb) noexcept {
        unsigned int count = 0;
        while (bb) {
            count++;
            bb &= bb - 1;
        }
        return static_cast<int>(count);
    }

    /// @brief Returns index of LSB bit
    /// @param bb The bitboard
    /// @return The index of the LSB bit
    [[maybe_unused]] [[nodiscard]] constexpr Square bit_scan(const Bitboard bb) noexcept {
        if (bb) {
            return (Square) bit_count((bb & -bb) - 1);
        } else {
            return EMPTY_SQUARE;
        }
    }

    /// @brief Returns index of LSB bit
    /// @param bb The bitboard
    /// @return The index of the LSB bit
    [[nodiscard]] constexpr Square bit_scan_forward(const Bitboard bb) noexcept {
        // clang-format off
        const int index64[64] = {0, 47, 1, 56, 48, 27, 2, 60, 57, 49, 41, 37, 28, 16, 3, 61, 54, 58, 35, 52, 50, 42, 21,
                                 44, 38, 32, 29, 23, 17, 11, 4, 62, 46, 55, 26, 59, 40, 36, 15, 53, 34, 51, 20, 43, 31,
                                 22, 10, 45, 25, 39, 14, 33, 19, 30, 9, 24, 13, 18, 8, 12, 7, 6, 5, 63};
        // clang-format on

        const Bitboard debruijn64 = 0x03f79d71b4cb0a89;
        return (Square) index64[((bb ^ (bb - 1)) * debruijn64) >> 58];
    }

    // -- Chess Specific Bitboard Functions --

    /// @brief The directions of the board represented as offsets
    enum Direction : int {
        NORTH [[maybe_unused]] = 8,
        SOUTH [[maybe_unused]] = -8,
        WEST [[maybe_unused]] = -1,
        EAST [[maybe_unused]] = 1,
        NORTH_WEST [[maybe_unused]] = 7,
        NORTH_EAST [[maybe_unused]] = 9,
        SOUTH_WEST [[maybe_unused]] = -9,
        SOUTH_EAST [[maybe_unused]] = -7,
        /// @brief Used for iterating over all directions
        N_DIRECTIONS [[maybe_unused]] = 8,
    };

    /// @brief Shifts a bitboard one square south
    /// @param bb The bitboard
    /// @return The shifted bitboard
    [[nodiscard]] constexpr Bitboard sout_one(const Bitboard bb) noexcept { return (bb >> Direction::NORTH); }

    /// @brief Shifts a bitboard one square north
    /// @param bb The bitboard
    /// @return The shifted bitboard
    [[nodiscard]] constexpr Bitboard nort_one(const Bitboard bb) noexcept { return (bb << Direction::NORTH); }

    /// @brief Shifts a bitboard one square east
    /// @param bb The bitboard
    /// @return The shifted bitboard
    [[nodiscard]] constexpr Bitboard east_one(const Bitboard bb) noexcept {
        return (bb << Direction::EAST) & 0xFEFEFEFEFEFEFEFE;
    }

    /// @brief Shifts a bitboard one square west
    /// @param bb The bitboard
    /// @return The shifted bitboard
    [[nodiscard]] constexpr Bitboard west_one(const Bitboard bb) noexcept {
        return (bb >> Direction::EAST) & 0x7F7F7F7F7F7F7F7F;
    }

    /// @brief Shifts a bitboard one square north east
    /// @param bb The bitboard
    /// @return The shifted bitboard
    [[nodiscard]] constexpr Bitboard no_ea_one(const Bitboard bb) noexcept {
        return (bb << Direction::NORTH_EAST) & 0xFEFEFEFEFEFEFEFE;
    }

    /// @brief Shifts a bitboard one square north west
    /// @param bb The bitboard
    /// @return The shifted bitboard
    [[nodiscard]] constexpr Bitboard no_we_one(const Bitboard bb) noexcept {
        return (bb << Direction::NORTH_WEST) & 0x7F7F7F7F7F7F7F7F;
    }

    /// @brief Shifts a bitboard one square south east
    /// @param bb The bitboard
    /// @return The shifted bitboard
    [[nodiscard]] constexpr Bitboard so_ea_one(const Bitboard bb) noexcept {
        return (bb >> Direction::NORTH_WEST) & 0xFEFEFEFEFEFEFEFE;
    }

    /// @brief Shifts a bitboard one square south west
    /// @param bb The bitboard
    /// @return The shifted bitboard
    [[nodiscard]] constexpr Bitboard so_we_one(const Bitboard bb) noexcept {
        return (bb >> Direction::NORTH_EAST) & 0x7F7F7F7F7F7F7F7F;
    }

    /// @brief Sets the occupancy bits
    /// @param index Index of the bitboard
    /// @param bits_in_mask Number of bits in the mask
    /// @param attack_mask The attack mask
    /// @return The occupancy bits
    [[nodiscard]] Bitboard set_occupancy(int index, int bits_in_mask, Bitboard attack_mask) noexcept;

    // -- Operator Overloads --

    /// @brief Prints a bitboard to the an output stream
    /// @param os The output stream
    /// @param bb The bitboard
    /// @return The output stream
    [[maybe_unused]] void Display(std::ostream &os, const std::uint64_t &bb) noexcept;
}// namespace bitboard
