#pragma once

#include <codchess/board.hpp>

namespace eval
{
    void init();

    /**
     * @brief Evaluates a given board
     * Positive values represent favorable position for white
     * Negative values represent favourable position for black
     *
     * @param board
     * @return absolute evaluation
     */
    int eval(const codchess::Board &board);

} // namespace eval