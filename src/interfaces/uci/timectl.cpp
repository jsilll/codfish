#include <interfaces/uci/timectl.hpp>

#include <engine/movegen/board.hpp>

#include <bits/stdc++.h>

namespace interfaces::uci::timectl
{
    int get_time_budget_ms(int wtime, int btime, const Board &board)
    {
        if (board.getSideToMove() == WHITE)
        {
            return wtime / std::max(40 - board.getFullMoveNumber(), 10);
        }
        else
        {
            return btime / std::max(40 - board.getFullMoveNumber(), 10);
        }
    }

} // namespace interfaces::uci::timectl
