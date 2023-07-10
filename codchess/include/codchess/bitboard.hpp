#pragma once

#include <iostream>

#include <codchess/base.hpp>

namespace cod::chess::bitboard {
/// @brief Bitboard type
typedef std::uint64_t Bitboard;

/// @brief Bitboard constant for bit manipulation
constexpr Bitboard kOne{0x0000000000000001};

/// @brief Bitboard constant for bit manipulation
constexpr Bitboard kZero{0x0000000000000000};

/// @brief Bitboard constant for bit manipulation
constexpr Bitboard kFull{0xFFFFFFFFFFFFFFFF};

/// @brief Bitboard constant for masking out the A file
constexpr Bitboard kNotAFile{0xFEFEFEFEFEFEFEFE};

/// @brief Bitboard constant for masking out the A file
constexpr Bitboard kNotHFile{0x7F7F7F7F7F7F7F7F};

/// @brief Bitboard Count type
typedef std::uint8_t BitboardCount;

/// @brief Sets a bit in a bitboard
/// @param bb The bitboard
/// @param sq The square
constexpr void
SetBit(Bitboard &bb, const Square sq) noexcept {
    bb |= (kOne << static_cast<std::uint8_t>(sq));
}

/// @brief Pops a bit in a bitboard
/// @param bn The bitboard
/// @param sq The square
constexpr void
PopBit(Bitboard &bb, const Square sq) noexcept {
    bb &= ~(kOne << static_cast<std::uint8_t>(sq));
}

/// @brief Pops the last bit in a bitboard
/// @param bb The bitboard
constexpr void
PopLastBit(Bitboard &bb) noexcept {
    bb &= bb - kOne;
}

/// @brief Gets a bit in a bitboard
/// @param bb The bitboard
/// @param sq The square
/// @return Whether the bit is set or not
constexpr bool
GetBit(const Bitboard bb, const Square sq) noexcept {
    return ((bb >> static_cast<std::uint8_t>(sq)) & kOne);
}

/// @brief Returns the number of bits in a bitboard
/// @param bb The bitboard
/// @return The number of bits in the bitboard
[[nodiscard]] constexpr BitboardCount
BitCount(Bitboard bb) noexcept {
    Bitboard count{0};
    while (bb) {
        ++count;
        bb &= bb - 1;
    }
    return count;
}

/// @brief Returns index of LSB bit
/// @param bb The bitboard
/// @return The index of the LSB bit
[[maybe_unused]] [[nodiscard]] constexpr Square
BitScan(const Bitboard bb) noexcept {
    if (bb) {
        return static_cast<Square>(BitCount((bb & -bb) - 1));
    } else {
        return Square::Empty;
    }
}

/// @brief Returns index of LSB bit
/// @param bb The bitboard
/// @return The square of the LSB bit
[[nodiscard]] Square BitScanForward(Bitboard bb) noexcept;

/// @brief The directions of the board represented as offsets
enum class Direction : std::int8_t {
    North [[maybe_unused]] = 8,
    South [[maybe_unused]] = -8,
    West [[maybe_unused]] = -1,
    East [[maybe_unused]] = 1,
    NorthWest [[maybe_unused]] = 7,
    NorthEast [[maybe_unused]] = 9,
    SouthWest [[maybe_unused]] = -9,
    SouthEast [[maybe_unused]] = -7,
    /// @brief Used for iterating over all directions
    Total [[maybe_unused]] = 8,
};

/// @brief Returns the bitboard shifted in a direction
/// @tparam D The direction
/// @param bb The bitboard
/// @return The shifted bitboard
template <Direction D>
[[nodiscard]] constexpr Bitboard
ShiftOne(const Bitboard bb) noexcept {
    if constexpr (D == Direction::North) {
        return bb << static_cast<std::int8_t>(Direction::North);
    } else if constexpr (D == Direction::South) {
        return bb >> static_cast<std::int8_t>(Direction::North);
    } else if constexpr (D == Direction::East) {
        return (bb << static_cast<std::int8_t>(Direction::East)) & kNotAFile;
    } else if constexpr (D == Direction::West) {
        return (bb >> static_cast<std::int8_t>(Direction::East)) & kNotHFile;
    } else if constexpr (D == Direction::NorthEast) {
        return (bb << static_cast<std::int8_t>(Direction::NorthEast)) &
               kNotAFile;
    } else if constexpr (D == Direction::NorthWest) {
        return (bb << static_cast<std::int8_t>(Direction::NorthWest)) &
               kNotHFile;
    } else if constexpr (D == Direction::SouthEast) {
        return (bb >> static_cast<std::int8_t>(Direction::NorthWest)) &
               kNotAFile;
    } else if constexpr (D == Direction::SouthWest) {
        return (bb >> static_cast<std::int8_t>(Direction::NorthEast)) &
               kNotHFile;
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
[[maybe_unused]] void Display(std::ostream &os, const Bitboard &bb) noexcept;
}   // namespace cod::chess::bitboard
