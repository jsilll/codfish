#pragma once

namespace interfaces::uci::timectl
{
    /**
     * @brief Get the time budget in milliseconds
     *
     * @param time
     * @param moves_played
     * @return long
     */
    long get_time_budget_ms(long time, int moves_played);
} // namespace interfaces::uci::timectl
