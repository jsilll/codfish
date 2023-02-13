#pragma once

#include <vector>

#include <codlib/constants.hpp>
#include <codlib/movegen/magics.hpp>

namespace tables
{
    /// @brief Initializes all the tables
    void init() noexcept;

    /// @brief Number of rook magics
    /// @note 4096 is the number of unique 12-bit numbers
    constexpr std::size_t N_ROOK_MAGICS = 4096;

    /// @brief Number of bishop magics
    /// @note 512 is the number of unique 9-bit numbers
    constexpr std::size_t N_BISHOP_MAGICS = 512;

    /// @brief Bitboard masks for each king attack
    /// @note The init() function must be called before using this
    auto ATTACKS_KING = std::vector<u64>(N_SQUARES);

    /// @brief Bitboard masks for each knight attack
    /// @note The init() function must be called before using this
    auto ATTACKS_KNIGHT = std::vector<u64>(N_SQUARES);

    /// @brief Bitboard masks for each pawn attack
    /// @note The init() function must be called before using this
    auto ATTACKS_PAWN = std::vector<std::vector<u64>>(N_SIDES, std::vector<u64>(N_SQUARES));

    /// @brief Bitboard masks for each rook attack
    /// @note The init() function must be called before using this
    auto ATTACKS_ROOK = std::vector<std::vector<u64>>(N_SQUARES, std::vector<u64>(N_ROOK_MAGICS));

    /// @brief Bitboard masks for each bishop attack
    /// @note The init() function must be called before using this
    auto ATTACKS_BISHOP = std::vector<std::vector<u64>>(N_SQUARES, std::vector<u64>(N_BISHOP_MAGICS));

    [[nodiscard]] constexpr u64 magic_index(u64 occ, const magics::Magic &magic) noexcept
    {
        occ &= magic.mask;
        occ *= magic.magic;
        occ >>= magic.shift;
        return occ;     
    }

    /// @brief Gets the bishop attacks for a given square and occupancy
    /// @param sq The square
    /// @param occ The occupancy
    /// @return The bishop attacks
    [[nodiscard]] u64 get_bishop_attacks(const Square sq, u64 occ) noexcept
    {
        const int idx = magic_index(occ, magics::MAGIC_TABLE_BISHOP[sq]);
        return ATTACKS_BISHOP[sq][idx];
    }

    /// @brief Gets the rook attacks for a given square and occupancy
    /// @param sq The square
    /// @param occ The occupancy
    /// @return The rook attacks
    [[nodiscard]] u64 get_rook_attacks(const Square sq, u64 occ) noexcept
    {
        magics::Magic magic = magics::MAGIC_TABLE_ROOK[sq];
        occ &= magic.mask;
        occ *= magic.magic;
        occ >>= magic.shift;
        return ATTACKS_ROOK[sq][occ];
    }

    /// @brief Gets the queen attacks for a given square and occupancy
    /// @param sq The square
    /// @param occ The occupancy
    /// @return The queen attacks
    [[nodiscard]] u64 get_queen_attacks(const Square sq, u64 occ) noexcept
    {
        return get_bishop_attacks(sq, occ) | get_rook_attacks(sq, occ);
    }
} // namespace tables