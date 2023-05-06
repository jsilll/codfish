#pragma once

class Board;

namespace uci::timectl
{
    /**
     * @brief Get the time budget in milliseconds
     *
     * @param wtime
     * @param btime
     * @param board
     * @return int
     */
    int get_time_budget_ms(int wtime, int btime, const Board &board);

} // namespace uci::timectl
