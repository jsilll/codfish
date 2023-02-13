#pragma once

#include <codlib/bitboard.hpp>

namespace attacks
{
    /// @brief Returns a bitboard of all white pawn single pushes.
    /// @param wpawns A bitboard of white pawns.
    /// @param empty A bitboard of empty squares.
    /// @return A bitboard of all white pawn single pushes.
    [[nodiscard]] u64 mask_white_pawn_single_pushes(u64 wpawns, u64 empty) noexcept;

    /// @brief Returns a bitboard of all black pawn single pushes.
    /// @param bpawns A bitboard of black pawns.
    /// @param empty A bitboard of empty squares.
    /// @return A bitboard of all black pawn single pushes.
    [[nodiscard]] u64 mask_black_pawn_single_pushes(u64 bpawns, u64 empty) noexcept;

    /// @brief Returns a bitboard of all white pawn double pushes.
    /// @param wpawns A bitboard of white pawns.
    /// @param empty A bitboard of empty squares.
    /// @return A bitboard of all white pawn double pushes.
    [[nodiscard]] u64 mask_white_pawn_double_pushes(u64 wpawns, u64 empty) noexcept;

    /// @brief Returns a bitboard of all black pawn double pushes.
    /// @param bpawns A bitboard of black pawns.
    /// @param empty A bitboard of empty squares.
    /// @return A bitboard of all black pawn double pushes.
    [[nodiscard]] u64 mask_black_pawn_double_pushes(u64 bpawns, u64 empty) noexcept;

    /// @brief Returns a bitboard of all the bishop attacks from a given square.
    /// @param sq The square to generate attacks from.
    /// @return A bitboard of all the bishop attacks from a given square.
    [[nodiscard]] u64 mask_bishop_attack_rays(Square sq) noexcept;

    /// @brief Returns a bitboard of all the rook attacks from a given square.
    /// @param sq The square to generate attacks from.
    /// @return A bitboard of all the rook attacks from a given square.
    [[nodiscard]] u64 mask_rook_attack_rays(Square sq) noexcept;

    /// @brief Returns a bitboard of all the white pawn attacks.
    /// @param wpawns A bitboard of white pawns.
    /// @return A bitboard of all the white pawn attacks.
    [[nodiscard]] u64 mask_white_pawn_any_attacks(u64 wpawns) noexcept;

    /// @brief Returns a bitboard of all the black pawn attacks.
    /// @param bpawns A bitboard of black pawns.
    /// @return A bitboard of all the black pawn attacks.
    [[nodiscard]] u64 mask_black_pawn_any_attacks(u64 bpawns) noexcept;

    /// @brief Returns a bitboard of all the knight attacks.
    /// @param knights A bitboard of knights.
    /// @return A bitboard of all the knight attacks.
    [[nodiscard]] u64 mask_knight_attacks(u64 knights) noexcept;

    /// @brief Returns a bitboard of all the king attacks.
    /// @param kings A bitboard of kings.
    /// @return A bitboard of all the king attacks.
    [[nodiscard]] u64 mask_king_attacks(u64 kings) noexcept;

    /// @brief Returns a bitboard of all the bishop attacks from a given square with a given block.
    /// @param sq The square to generate attacks from.
    /// @param block A bitboard of the blocked squares.
    /// @return A bitboard of all the bishop attacks.
    [[nodiscard]] u64 mask_bishop_xray_attacks(Square sq, u64 block) noexcept;

    /// @brief Returns a bitboard of all the rook attacks from a given square with a given block.
    /// @param sq The square to generate attacks from.
    /// @param block A bitboard of the blocked squares.
    /// @return A bitboard of all the rook attacks.
    [[nodiscard]] u64 mask_rook_xray_attacks(Square sq, u64 block) noexcept;
} // namespace attacks