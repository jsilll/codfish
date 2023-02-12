#pragma once

#include <codlib/constants.hpp>
#include <codlib/board.hpp>
#include <codlib/bitboard.hpp>

namespace zobrist
{
    void init();

    extern u64 piece_keys[N_SIDES][N_PIECES][N_SQUARES];
    extern u64 en_passant_keys[N_SQUARES];
    extern u64 castle_keys[16];
    extern u64 side_key[BOTH];

    u64 generate_hash_key(const Board &board);

} // namespace zobrist
