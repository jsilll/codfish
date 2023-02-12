#include <interfaces/uci/timectl.hpp>

#include <codlib/board.hpp>

namespace uci::timectl
{
    int get_time_budget_ms(int wtime, int btime, const Board &board)
    {
        int remaining_time = board.get_side_to_move() == WHITE ? wtime : btime;
        int remaining_moves_pred = board.get_full_move_number() < 40 ? 40 - board.get_full_move_number() : std::max(80 - board.get_full_move_number(), 10);
        return remaining_time / remaining_moves_pred;
    }

} // namespace uci::timectl
