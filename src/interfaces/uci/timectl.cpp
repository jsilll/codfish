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

    int get_timeout_ms(int depth, int wtime, int btime, const Board &board)
    {
        return (int)std::pow(get_time_budget_ms(wtime, btime, board) / 3072, depth);
    }

} // namespace interfaces::uci::timectl
