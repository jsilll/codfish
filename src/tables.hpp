#pragma once

#include "defs.hpp"

namespace Tables
{
    constexpr U64 MASK_RANK[] = {
        0xFF,
        0xFF00,
        0xFF0000,
        0xFF000000,
        0xFF00000000,
        0xFF0000000000,
        0xFF000000000000,
        0xFF00000000000000};

    constexpr U64 MASK_FILE[] = {
        0x0101010101010101,
        0x202020202020202,
        0x404040404040404,
        0x808080808080808,
        0x1010101010101010,
        0x2020202020202020,
        0x4040404040404040,
        0x8080808080808080};

    constexpr U64 MASK_CLEAR_RANK[] = {
        0xFFFFFFFFFFFFFF00,
        0xFFFFFFFFFFFF00FF,
        0xFFFFFFFFFF00FFFF,
        0xFFFFFFFF00FFFFFF,
        0xFFFFFF00FFFFFFFF,
        0xFFFF00FFFFFFFFFF,
        0xFF00FFFFFFFFFFFF,
        0x00FFFFFFFFFFFFFF};

    constexpr U64 MASK_CLEAR_FILE[] = {
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
        6,5,5,5,5,5,5,6,
        5,5,5,5,5,5,5,5,
        5,5,7,7,7,7,5,5,
        5,5,7,9,9,7,5,5,
        5,5,7,9,9,7,5,5,
        5,5,7,7,7,7,5,5,
        5,5,5,5,5,5,5,5,
        6,5,5,5,5,5,5,6,
    };

    constexpr int RELEVANT_BITS_COUNT_ROOK[N_SQUARES] = {
        12,11,11,11,11,11,11,12,
        11,10,10,10,10,10,10,11,
        11,10,10,10,10,10,10,11,
        11,10,10,10,10,10,10,11,
        11,10,10,10,10,10,10,11,
        11,10,10,10,10,10,10,11,
        11,10,10,10,10,10,10,11,
        12,11,11,11,11,11,11,12,
    };
    // clang-format on

    extern U64 SQUARE_BB[N_SQUARES];
    extern U64 ATTACKS_PAWN[N_SIDES][N_SQUARES];
    extern U64 ATTACKS_KNIGHT[N_SQUARES];
    extern U64 ATTACKS_KING[N_SQUARES];
    extern U64 ATTACKS_BISHOP[N_SQUARES][512];
    extern U64 ATTACKS_ROOK[N_SQUARES][4096];

    void init();
    U64 maskBishopAttacks(int sq, U64 block);
    U64 maskRookAttacks(int sq, U64 block);
    U64 maskRookAttackRays(int sq);
    U64 maskBishopAttackRays(int sq);

} // namespace Tables