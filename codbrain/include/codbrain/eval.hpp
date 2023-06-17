#pragma once

#include <codchess/board.hpp>

namespace codbrain::eval
{
    void Init() noexcept;

    /**
     * @brief Evaluates a given board
     * Positive values represent favorable position for white
     * Negative values represent favourable position for black
     *
     * @param board
     * @return absolute evaluation
     */
    int Static(const codchess::Board &board) noexcept;

} // namespace codbrain::eval