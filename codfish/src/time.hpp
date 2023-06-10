#include <codchess/board.hpp>

namespace codfish::time {
int
ComputeTimeBudgetMs(int wtime, int btime, const codchess::Board &board) {
    const auto remaining_time =
        board.active() == codchess::WHITE ? wtime : btime;
    const auto remaining_moves_pred =
        board.full_move_number() < 40
            ? 40 - board.full_move_number()
            : std::max(80 - board.full_move_number(), 10);
    return remaining_time / remaining_moves_pred;
}
}   // namespace codfish::time
