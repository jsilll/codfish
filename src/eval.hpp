#pragma once

#include "defs.hpp"

class Board;

namespace Eval
{
    void init();

    /**
     * @brief Evaluates a given board
     *
     * @param board
     * @return int
     */
    int eval(Board &board);

} // namespace Eval