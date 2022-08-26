#pragma once

#include <engine/constants.hpp>

class Board;

namespace perft
{
    /**
     * @brief perft Function
     *
     * @param depth
     * @return int
     */
    int perft(Board &board, int depth);

} // namespace perft