#pragma once

#include "defs.hpp"

class Board;

namespace Perft
{
    /**
     * @brief Perft Function
     *
     * @param depth
     * @return long long
     */
    unsigned long long perft(const Board &board, int depth);
}