#pragma once

#include <engine/defs.hpp>

class Board;

namespace perft
{
    /**
     * @brief perft Function
     *
     * @param depth
     * @return long long
     */
    unsigned long long perft(const Board &board, int depth);

} // namespace perft