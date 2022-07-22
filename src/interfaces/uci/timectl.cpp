#include <interfaces/uci/timectl.hpp>

#include <engine/board.hpp>

#include <bits/stdc++.h>

namespace uci::timectl
{
    int get_time_budget_ms(int wtime, int btime, const Board &board)
    {
        int remaining_time = board.get_side_to_move() == WHITE ? wtime : btime;
        return remaining_time / std::max(40 - board.get_full_move_number(), 10);
    }

} // namespace uci::timectl
