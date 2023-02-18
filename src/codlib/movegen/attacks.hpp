#pragma once

#include <vector>

#include <codlib/bitboard.hpp>
#include <codlib/movegen/magics.hpp>
#include <codlib/utils.hpp>

namespace attacks {
    /// @brief Initializes all the attack tables
    void init() noexcept;

    /// @brief Number of rook magics
    /// @note 4096 is the number of unique 12-bit numbers
    constexpr std::size_t N_ROOK_MAGICS = 4096;

    /// @brief Number of bishop magics
    /// @note 512 is the number of unique 9-bit numbers
    constexpr std::size_t N_BISHOP_MAGICS = 512;

    /// @brief Bitboard masks for each king attack
    /// @note The init() function must be called before using this
    extern bitboard::Bitboard ATTACKS_KING[Square::N_SQUARES];

    /// @brief Bitboard masks for each knight attack
    /// @note The init() function must be called before using this
    extern bitboard::Bitboard ATTACKS_KNIGHT[Square::N_SQUARES];

    /// @brief Bitboard masks for each pawn attack
    /// @note The init() function must be called before using this
    extern bitboard::Bitboard ATTACKS_PAWN[Color::N_COLORS][Square::N_SQUARES];

    /// @brief Returns a bitboard of all east attacks of white pawns.
    /// @param wpawns A bitboard of white pawns.
    /// @return A bitboard of all east attacks of white pawns.
    [[nodiscard]] constexpr bitboard::Bitboard mask_white_pawn_east_attacks(const bitboard::Bitboard wpawns) noexcept {
        return bitboard::no_ea_one(wpawns);
    }

    /// @brief Returns a bitboard of all west attacks of white pawns.
    /// @param wpawns A bitboard of white pawns.
    /// @return A bitboard of all west attacks of white pawns.
    [[nodiscard]] constexpr bitboard::Bitboard mask_white_pawn_west_attacks(const bitboard::Bitboard wpawns) noexcept {
        return bitboard::no_we_one(wpawns);
    }

    /// @brief Returns a bitboard of all east attacks of black pawns.
    /// @param bpawns A bitboard of black pawns.
    /// @return A bitboard of all east attacks of black pawns.
    [[nodiscard]] constexpr bitboard::Bitboard mask_black_pawn_east_attacks(const bitboard::Bitboard bpawns) noexcept {
        return bitboard::so_ea_one(bpawns);
    }

    /// @brief Returns a bitboard of all west attacks of black pawns.
    /// @param bpawns A bitboard of black pawns.
    /// @return A bitboard of all west attacks of black pawns.
    [[nodiscard]] constexpr bitboard::Bitboard mask_black_pawn_west_attacks(const bitboard::Bitboard bpawns) noexcept {
        return bitboard::so_we_one(bpawns);
    }

    /// @brief Returns a bitboard of all white pawn single pushes.
    /// @param wpawns A bitboard of white pawns.
    /// @param empty A bitboard of empty squares.
    /// @return A bitboard of all white pawn single pushes.
    [[nodiscard]] constexpr bitboard::Bitboard
    mask_white_pawn_single_pushes(const bitboard::Bitboard wpawns, const bitboard::Bitboard empty) noexcept {
        return bitboard::nort_one(wpawns) & empty;
    }

    /// @brief Returns a bitboard of all black pawn single pushes.
    /// @param bpawns A bitboard of black pawns.
    /// @param empty A bitboard of empty squares.
    /// @return A bitboard of all black pawn single pushes.
    [[nodiscard]] constexpr bitboard::Bitboard
    mask_black_pawn_single_pushes(const bitboard::Bitboard bpawns, const bitboard::Bitboard empty) noexcept {
        return bitboard::sout_one(bpawns) & empty;
    }

    /// @brief Returns a bitboard of all white pawn double pushes.
    /// @param wpawns A bitboard of white pawns.
    /// @param empty A bitboard of empty squares.
    /// @return A bitboard of all white pawn double pushes.
    [[nodiscard]] constexpr bitboard::Bitboard
    mask_white_pawn_double_pushes(const bitboard::Bitboard wpawns, const bitboard::Bitboard empty) noexcept {
        bitboard::Bitboard single_pushes = mask_white_pawn_single_pushes(wpawns, empty);
        return bitboard::nort_one(single_pushes) & empty & utils::MASK_RANK[RANK_3];
    }

    /// @brief Returns a bitboard of all black pawn double pushes.
    /// @param bpawns A bitboard of black pawns.
    /// @param empty A bitboard of empty squares.
    /// @return A bitboard of all black pawn double pushes.
    [[nodiscard]] constexpr bitboard::Bitboard
    mask_black_pawn_double_pushes(const bitboard::Bitboard bpawns, const bitboard::Bitboard empty) noexcept {
        const bitboard::Bitboard single_pushes = mask_black_pawn_single_pushes(bpawns, empty);
        return bitboard::sout_one(single_pushes) & empty & utils::MASK_RANK[RANK_4];
    }

    /// @brief Returns a bitboard of all the white pawn attacks.
    /// @param wpawns A bitboard of white pawns.
    /// @return A bitboard of all the white pawn attacks.
    [[nodiscard]] constexpr bitboard::Bitboard mask_white_pawn_any_attacks(const bitboard::Bitboard wpawns) noexcept {
        return mask_white_pawn_east_attacks(wpawns) | mask_white_pawn_west_attacks(wpawns);
    }

    /// @brief Returns a bitboard of all the black pawn attacks.
    /// @param bpawns A bitboard of black pawns.
    /// @return A bitboard of all the black pawn attacks.
    [[nodiscard]] constexpr bitboard::Bitboard mask_black_pawn_any_attacks(const bitboard::Bitboard bpawns) noexcept {
        return mask_black_pawn_east_attacks(bpawns) | mask_black_pawn_west_attacks(bpawns);
    }

    /// @brief Returns a bitboard of all the bishop attacks from a given square with
    /// a given block.
    /// @param sq The square to generate attacks from.
    /// @param block A bitboard of the blocked squares.
    /// @return A bitboard of all the bishop attacks.
    [[nodiscard]] constexpr bitboard::Bitboard
    mask_bishop_xray_attacks(const Square sq, const bitboard::Bitboard block) noexcept {
        bitboard::Bitboard attacks = bitboard::ZERO;
        Rank rank = utils::get_rank(sq);
        File file = utils::get_file(sq);
        for (int r = rank + 1, f = file + 1; r < N_RANKS && f < N_FILES; ++r, ++f) {
            attacks |= (bitboard::ONE << utils::get_square((Rank) r, (File) f));
            if ((bitboard::ONE << utils::get_square((Rank) r, (File) f)) & block) { break; }
        }

        for (int r = rank + 1, f = file - 1; r < N_RANKS && f >= FILE_A; ++r, --f) {
            attacks |= (bitboard::ONE << utils::get_square((Rank) r, (File) f));
            if ((bitboard::ONE << utils::get_square((Rank) r, (File) f)) & block) { break; }
        }

        for (int r = rank - 1, f = file + 1; r >= RANK_1 && f < N_FILES; --r, ++f) {
            attacks |= (bitboard::ONE << utils::get_square((Rank) r, (File) f));
            if ((bitboard::ONE << utils::get_square((Rank) r, (File) f)) & block) { break; }
        }

        for (int r = rank - 1, f = file - 1; r >= RANK_1 && f >= FILE_A; --r, --f) {
            attacks |= (bitboard::ONE << utils::get_square((Rank) r, (File) f));
            if ((bitboard::ONE << utils::get_square((Rank) r, (File) f)) & block) { break; }
        }

        return attacks;
    }

    /// @brief Returns a bitboard of all the rook attacks from a given square with a
    /// given block.
    /// @param sq The square to generate attacks from.
    /// @param block A bitboard of the blocked squares.
    /// @return A bitboard of all the rook attacks.
    [[nodiscard]] constexpr bitboard::Bitboard
    mask_rook_xray_attacks(const Square sq, const bitboard::Bitboard block) noexcept {
        bitboard::Bitboard attacks = bitboard::ZERO;
        Rank rank = utils::get_rank(sq);
        File file = utils::get_file(sq);
        for (int r = rank + 1; r < N_RANKS; ++r) {
            attacks |= (bitboard::ONE << utils::get_square((Rank) r, file));
            if ((bitboard::ONE << utils::get_square((Rank) r, file)) & block) { break; }
        }

        for (int r = rank - 1; r >= RANK_1; --r) {
            attacks |= (bitboard::ONE << utils::get_square((Rank) r, file));
            if ((bitboard::ONE << utils::get_square((Rank) r, file)) & block) { break; }
        }

        for (int f = file + 1; f < N_FILES; ++f) {
            attacks |= (bitboard::ONE << utils::get_square(rank, (File) f));
            if ((bitboard::ONE << utils::get_square(rank, (File) f)) & block) { break; }
        }

        for (int f = file - 1; f >= FILE_A; --f) {
            attacks |= (bitboard::ONE << utils::get_square(rank, (File) f));
            if ((bitboard::ONE << utils::get_square(rank, (File) f)) & block) { break; }
        }

        return attacks;
    }

    /// @brief Gets the bishop attacks for a given square and occupancy
    /// @param sq The square
    /// @param occ The occupancy
    /// @return The bishop attacks
    [[nodiscard]] bitboard::Bitboard get_bishop_attacks(Square sq, bitboard::Bitboard occ) noexcept;

    /// @brief Gets the rook attacks for a given square and occupancy
    /// @param sq The square
    /// @param occ The occupancy
    /// @return The rook attacks
    [[nodiscard]] bitboard::Bitboard get_rook_attacks(Square sq, bitboard::Bitboard occ) noexcept;

    /// @brief Gets the queen attacks for a given square and occupancy
    /// @param sq The square
    /// @param occ The occupancy
    /// @return The queen attacks
    [[nodiscard]] inline bitboard::Bitboard get_queen_attacks(const Square sq, const bitboard::Bitboard occ) noexcept {
        return get_bishop_attacks(sq, occ) | get_rook_attacks(sq, occ);
    }
}// namespace attacks