#pragma once

class Board;

namespace Evaluation
{
    /**
     * @brief Evaluates a given board
     *
     * @param board
     * @return int
     */
    int eval(Board &board);

} // namespace Evaluation