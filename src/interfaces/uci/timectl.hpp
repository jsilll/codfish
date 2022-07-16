#pragma once

class Board;

namespace interfaces::uci::timectl
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

    /**
     * @brief Get the timeout in milliseconds
     *
     * @param wtime
     * @param btim
     * @param board
     * @return int
     */
    int get_timeout_ms(int depth, int wtime, int btime, const Board &board);

} // namespace interfaces::uci::timectl
