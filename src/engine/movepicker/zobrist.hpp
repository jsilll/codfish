#pragma once

#include <engine/constants.hpp>
#include <engine/board.hpp>
#include <engine/bitboard.hpp>

namespace zobrist
{
    void init();

    u64 generate_hash_key(const Board &board);

} // namespace zobrist
