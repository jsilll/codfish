#pragma once

#include <engine/constants.hpp>

#include <engine/movegen/magics.hpp>

namespace tables
{
    constexpr u64 MASK_RANK[] = {
        0xFF,
        0xFF00,
        0xFF0000,
        0xFF000000,
        0xFF00000000,
        0xFF0000000000,
        0xFF000000000000,
        0xFF00000000000000};

    constexpr u64 MASK_FILE[] = {
        0x0101010101010101,
        0x202020202020202,
        0x404040404040404,
        0x808080808080808,
        0x1010101010101010,
        0x2020202020202020,
        0x4040404040404040,
        0x8080808080808080};

    constexpr u64 MASK_CLEAR_RANK[] = {
        0xFFFFFFFFFFFFFF00,
        0xFFFFFFFFFFFF00FF,
        0xFFFFFFFFFF00FFFF,
        0xFFFFFFFF00FFFFFF,
        0xFFFFFF00FFFFFFFF,
        0xFFFF00FFFFFFFFFF,
        0xFF00FFFFFFFFFFFF,
        0x00FFFFFFFFFFFFFF};

    constexpr u64 MASK_CLEAR_FILE[] = {
        0xFEFEFEFEFEFEFEFE,
        0xFDFDFDFDFDFDFDFD,
        0xFBFBFBFBFBFBFBFB,
        0xF7F7F7F7F7F7F7F7,
        0xEFEFEFEFEFEFEFEF,
        0xDFDFDFDFDFDFDFDF,
        0xBFBFBFBFBFBFBFBF,
        0x7F7F7F7F7F7F7F7F};

    // clang-format off
    constexpr int RELEVANT_BITS_COUNT_BISHOP[N_SQUARES] = {
        6, 5, 5, 5, 5, 5, 5, 6,
        5, 5, 5, 5, 5, 5, 5, 5,
        5, 5, 7, 7, 7, 7, 5, 5,
        5, 5, 7, 9, 9, 7, 5, 5,
        5, 5, 7, 9, 9, 7, 5, 5,
        5, 5, 7, 7, 7, 7, 5, 5,
        5, 5, 5, 5, 5, 5, 5, 5,
        6, 5, 5, 5, 5, 5, 5, 6,
    };

    constexpr int RELEVANT_BITS_COUNT_ROOK[N_SQUARES] = {
        12, 11, 11, 11, 11, 11, 11, 12,
        11, 10, 10, 10, 10, 10, 10, 11,
        11, 10, 10, 10, 10, 10, 10, 11,
        11, 10, 10, 10, 10, 10, 10, 11,
        11, 10, 10, 10, 10, 10, 10, 11,
        11, 10, 10, 10, 10, 10, 10, 11,
        11, 10, 10, 10, 10, 10, 10, 11,
        12, 11, 11, 11, 11, 11, 11, 12,
    };
    // clang-format on

    extern u64 SQUARE_BB[N_SQUARES];

    extern u64 ATTACKS_PAWN[BOTH][N_SQUARES]; // Direct Access TODO: make this accessible through function only?
    extern u64 ATTACKS_KNIGHT[N_SQUARES];     // Direct Access
    extern u64 ATTACKS_KING[N_SQUARES];       // Direct Access

    extern u64 ATTACKS_BISHOP[N_SQUARES][512]; // Needs Magics Bitboards For Accessing
    extern u64 ATTACKS_ROOK[N_SQUARES][4096];  // Needs Magics Bitboards For Accessing

    void init();

    inline u64 get_bishop_attacks(const int sq, u64 occ)
    {
        occ &= magics::MAGIC_TABLE_BISHOP[sq].mask;
        occ *= magics::MAGIC_TABLE_BISHOP[sq].magic;
        occ >>= magics::MAGIC_TABLE_BISHOP[sq].shift;
        return ATTACKS_BISHOP[sq][occ];
    }

    inline u64 get_rook_attacks(const int sq, u64 occ)
    {
        occ &= magics::MAGIC_TABLE_ROOK[sq].mask;
        occ *= magics::MAGIC_TABLE_ROOK[sq].magic;
        occ >>= magics::MAGIC_TABLE_ROOK[sq].shift;
        return ATTACKS_ROOK[sq][occ];
    }

    inline u64 get_queen_attacks(const int sq, u64 occ)
    {
        return get_bishop_attacks(sq, occ) | get_rook_attacks(sq, occ);
    }

} // namespace tables