#pragma once

#include <cstdint>
#include <iostream>

#include <codchess/base.hpp>

namespace codchess::bitboard {
/// @brief Bitboard type
typedef std::uint64_t Bitboard;

/// @brief Bitboard constant for bit manipulation
constexpr Bitboard ONE{1};

/// @brief Bitboard constant for bit manipulation
constexpr Bitboard ZERO{0};

/// @brief Sets a bit in a bitboard
/// @param bb The bitboard
/// @param sq The square
constexpr void
SetBit(Bitboard &bb, Square sq) noexcept {
    bb |= (ONE << sq);
}

/// @brief Pops a bit in a bitboard
/// @param bn The bitboard
/// @param sq The square
constexpr void
PopBit(Bitboard &bb, Square sq) noexcept {
    bb &= ~(ONE << sq);
}

/// @brief Pops the last bit in a bitboard
/// @param bb The bitboard
constexpr void
PopLastBit(Bitboard &bb) noexcept {
    bb &= bb - ONE;
}

/// @brief Gets a bit in a bitboard
/// @param bb The bitboard
/// @param sq The square
/// @return Whether the bit is set or not
[[nodiscard]] constexpr bool
GetBit(const Bitboard bb, const Square sq) noexcept {
    return ((bb >> sq) & ONE);
}

/// @brief Returns the number of bits in a bitboard
/// @param bb The bitboard
/// @return The number of bits in the bitboard
[[nodiscard]] constexpr auto
BitCount(Bitboard bb) noexcept {
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
[[maybe_unused]] [[nodiscard]] constexpr auto
BitScan(const Bitboard bb) noexcept {
    if (bb) {
        return static_cast<Square>(BitCount((bb & -bb) - 1));
    } else {
        return EMPTY_SQUARE;
    }
}

/// @brief Returns index of LSB bit
/// @param bb The bitboard
/// @return The square of the LSB bit
[[nodiscard]] Square BitScanForward(const Bitboard bb) noexcept;

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

/// @brief Returns the bitboard shifted in a direction
/// @tparam D The direction
/// @param bb The bitboard
/// @return The shifted bitboard
template <Direction D>
[[nodiscard]] constexpr Bitboard
ShiftOne(const Bitboard bb) noexcept {
    if constexpr (D == Direction::NORTH) {
        return bb << Direction::NORTH;
    } else if constexpr (D == Direction::SOUTH) {
        return bb >> Direction::NORTH;
    } else if constexpr (D == Direction::EAST) {
        return (bb << Direction::EAST) & 0xFEFEFEFEFEFEFEFE;
    } else if constexpr (D == Direction::WEST) {
        return (bb >> Direction::EAST) & 0x7F7F7F7F7F7F7F7F;
    } else if constexpr (D == Direction::NORTH_EAST) {
        return (bb << Direction::NORTH_EAST) & 0xFEFEFEFEFEFEFEFE;
    } else if constexpr (D == Direction::NORTH_WEST) {
        return (bb << Direction::NORTH_WEST) & 0x7F7F7F7F7F7F7F7F;
    } else if constexpr (D == Direction::SOUTH_EAST) {
        return (bb >> Direction::NORTH_WEST) & 0xFEFEFEFEFEFEFEFE;
    } else if constexpr (D == Direction::SOUTH_WEST) {
        return (bb >> Direction::NORTH_EAST) & 0x7F7F7F7F7F7F7F7F;
    }
}

/// @brief Sets the occupancy bits
/// @param index Index of the bitboard
/// @param bits_in_mask Number of bits in the mask
/// @param attack_mask The attack mask
/// @return The occupancy bits
[[nodiscard]] Bitboard SetOccupancy(int index, int bits_in_mask,
                                    Bitboard attack_mask) noexcept;

/// @brief Prints a bitboard to the an output stream
/// @param os The output stream
/// @param bb The bitboard
/// @return The output stream
[[maybe_unused]] void Display(std::ostream &os,
                              const std::uint64_t &bb) noexcept;
}   // namespace codchess::bitboard
