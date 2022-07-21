#include <interfaces/uci/timectl.hpp>

#include <engine/movegen/board.hpp>

#include <bits/stdc++.h>

namespace interfaces::uci::timectl
{
    int get_time_budget_ms(int wtime, int btime, const Board &board)
    {
        int remaining_time = board.getSideToMove() == WHITE ? wtime : btime;
        return remaining_time / std::max(40 - board.getFullMoveNumber(), 10);
    }

} // namespace interfaces::uci::timectl
