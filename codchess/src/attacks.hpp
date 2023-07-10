#pragma once

#include <vector>

#include <codchess/bitboard.hpp>
#include <codchess/utils.hpp>

#include "magics.hpp"

namespace cod::chess::attacks {
/// @brief Initializes all the attack tables
void Init() noexcept;

/// @brief Number of rook magics
/// @note 4096 is the number of unique 12-bit numbers
constexpr std::size_t kRookMagics{4096};

/// @brief Number of bishop magics
/// @note 512 is the number of unique 9-bit numbers
constexpr std::size_t kBishopMagics{512};

/// @brief Bitboard masks for each king attack
/// @note The Init() function must be called before using this
extern bitboard::Bitboard
    kKingAttacks[static_cast<std::size_t>(Square::Total)];

/// @brief Bitboard masks for each knight attack
/// @note The Init() function must be called before using this
extern bitboard::Bitboard
    kKnightAttacks[static_cast<std::size_t>(Square::Total)];

/// @brief Bitboard masks for each pawn attack
/// @note The Init() function must be called before using this
extern bitboard::Bitboard kPawnAttacks[static_cast<std::size_t>(
    Color::Total)][static_cast<std::size_t>(Square::Total)];

/// @brief Masks the pawn attacks for a given color and direction.
/// @tparam C The color of the pawns.
/// @tparam D The direction of the pawn attacks.
/// @param pawns A bitboard of pawns.
/// @return A bitboard of all the pawn attacks.
template <Color C, bitboard::Direction D>
[[nodiscard]] constexpr bitboard::Bitboard
PawnAttacks(const bitboard::Bitboard pawns) noexcept {
    static_assert(C == Color::White or C == Color::Black);
    static_assert(D == bitboard::Direction::West ||
                  D == bitboard::Direction::East);

    if constexpr (C == Color::White) {
        if constexpr (D == bitboard::Direction::West) {
            return bitboard::ShiftOne<bitboard::Direction::NorthWest>(pawns);
        } else {
            return bitboard::ShiftOne<bitboard::Direction::NorthEast>(pawns);
        }
    } else {
        if constexpr (D == bitboard::Direction::West) {
            return bitboard::ShiftOne<bitboard::Direction::SouthWest>(pawns);
        } else {
            return bitboard::ShiftOne<bitboard::Direction::SouthEast>(pawns);
        }
    }
}

template <Color C>
[[nodiscard]] constexpr bitboard::Bitboard
PawnAllAttacks(const bitboard::Bitboard pawns) noexcept {
    static_assert(C == Color::White or C == Color::Black);

    return PawnAttacks<C, bitboard::Direction::West>(pawns) |
           PawnAttacks<C, bitboard::Direction::East>(pawns);
}

/// @brief Returns a bitboard of all pawn single pushes.
/// @tparam C The color of the pawns.
/// @param pawns A bitboard of pawns.
/// @param empty A bitboard of empty squares.
/// @return A bitboard of all pawn single pushes.
template <Color C>
[[nodiscard]] constexpr bitboard::Bitboard
PawnSinglePushes(const bitboard::Bitboard pawns,
                 const bitboard::Bitboard empty) noexcept {
    static_assert(C == Color::White or C == Color::Black);

    if constexpr (C == Color::White) {
        return bitboard::ShiftOne<bitboard::Direction::North>(pawns) & empty;
    } else {
        return bitboard::ShiftOne<bitboard::Direction::South>(pawns) & empty;
    }
}

/// @brief Returns a bitboard of all pawn double pushes.
/// @tparam C The color of the pawns.
/// @param pawns A bitboard of pawns.
/// @param empty A bitboard of empty squares.
/// @return A bitboard of all pawn double pushes.
template <Color C>
[[nodiscard]] constexpr bitboard::Bitboard
PawnDoublePushes(const bitboard::Bitboard pawns,
                 const bitboard::Bitboard empty) noexcept {
    static_assert(C == Color::White or C == Color::Black);

    const auto pushes =
        PawnSinglePushes<C>(PawnSinglePushes<C>(pawns, empty), empty);
    if constexpr (C == Color::White) {
        return pushes &
               utils::kMaskRank[static_cast<std::size_t>(Rank::R4)];
    } else {
        return pushes &
               utils::kMaskRank[static_cast<std::size_t>(Rank::R5)];
    }
}

/// @brief Gets the bishop attacks for a given square and occupancy
/// @param sq The square
/// @param block The occupancy
/// @return The bishop attacks
[[nodiscard]] bitboard::Bitboard
BishopAttacks(Square sq, bitboard::Bitboard block) noexcept;

/// @brief Gets the rook attacks for a given square and occupancy
/// @param sq The square
/// @param block The occupancy
/// @return The rook attacks
[[nodiscard]] bitboard::Bitboard RookAttacks(Square sq,
                                             bitboard::Bitboard block) noexcept;

/// @brief Gets the queen attacks for a given square and occupancy
/// @param sq The square
/// @param occ The occupancy
/// @return The queen attacks
[[nodiscard]] inline bitboard::Bitboard
QueenAttacks(const Square sq, const bitboard::Bitboard occ) noexcept {
    return BishopAttacks(sq, occ) | RookAttacks(sq, occ);
}
}   // namespace cod::chess::attacks