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
constexpr std::size_t N_ROOK_MAGICS{4096};

/// @brief Number of bishop magics
/// @note 512 is the number of unique 9-bit numbers
constexpr std::size_t N_BISHOP_MAGICS{512};

/// @brief Bitboard masks for each king attack
/// @note The Init() function must be called before using this
extern bitboard::Bitboard
    KING_ATTACKS[static_cast<std::size_t>(Square::N_SQUARES)];

/// @brief Bitboard masks for each knight attack
/// @note The Init() function must be called before using this
extern bitboard::Bitboard
    KNIGHT_ATTACKS[static_cast<std::size_t>(Square::N_SQUARES)];

/// @brief Bitboard masks for each pawn attack
/// @note The Init() function must be called before using this
extern bitboard::Bitboard PAWN_ATTACKS[static_cast<std::size_t>(
    Color::N_COLORS)][static_cast<std::size_t>(Square::N_SQUARES)];

/// @brief Masks the pawn attacks for a given color and direction.
/// @tparam C The color of the pawns.
/// @tparam D The direction of the pawn attacks.
/// @param pawns A bitboard of pawns.
/// @return A bitboard of all the pawn attacks.
template <Color C, bitboard::Direction D>
[[nodiscard]] constexpr bitboard::Bitboard
PawnAttacks(const bitboard::Bitboard pawns) noexcept {
    static_assert(C == Color::WHITE or C == Color::BLACK);
    static_assert(D == bitboard::Direction::WEST ||
                  D == bitboard::Direction::EAST);

    if constexpr (C == Color::WHITE) {
        if constexpr (D == bitboard::Direction::WEST) {
            return bitboard::ShiftOne<bitboard::Direction::NORTH_WEST>(pawns);
        } else {
            return bitboard::ShiftOne<bitboard::Direction::NORTH_EAST>(pawns);
        }
    } else {
        if constexpr (D == bitboard::Direction::WEST) {
            return bitboard::ShiftOne<bitboard::Direction::SOUTH_WEST>(pawns);
        } else {
            return bitboard::ShiftOne<bitboard::Direction::SOUTH_EAST>(pawns);
        }
    }
}

template <Color C>
[[nodiscard]] constexpr bitboard::Bitboard
PawnAllAttacks(const bitboard::Bitboard pawns) noexcept {
    static_assert(C == Color::WHITE or C == Color::BLACK);

    return PawnAttacks<C, bitboard::Direction::WEST>(pawns) |
           PawnAttacks<C, bitboard::Direction::EAST>(pawns);
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
    static_assert(C == Color::WHITE or C == Color::BLACK);

    if constexpr (C == Color::WHITE) {
        return bitboard::ShiftOne<bitboard::Direction::NORTH>(pawns) & empty;
    } else {
        return bitboard::ShiftOne<bitboard::Direction::SOUTH>(pawns) & empty;
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
    static_assert(C == Color::WHITE or C == Color::BLACK);

    const auto pushes =
        PawnSinglePushes<C>(PawnSinglePushes<C>(pawns, empty), empty);

    if constexpr (C == Color::WHITE) {
        return pushes &
               utils::MASK_RANK[static_cast<std::size_t>(Rank::RANK_4)];
    } else {
        return pushes &
               utils::MASK_RANK[static_cast<std::size_t>(Rank::RANK_5)];
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