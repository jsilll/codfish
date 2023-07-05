#include <codbrain/codbrain.hpp>

#include <algorithm>

using namespace codchess;

namespace codbrain {
Evaluation
Rollout(Board &board) noexcept {
    auto moves = movegen::Legal(board);
    if (moves.empty()) {
        return board.IsCheck() ? -1 : 0;
    } else {
        std::random_shuffle(moves.begin(), moves.end());
        const auto backup = board.GetStateBackup();
        board.Make(*moves.begin());
        auto eval = -Rollout(board);
        board.Unmake(*moves.begin(), backup);
        return eval;
    }
}

Result
Mcts::PickMove() noexcept {
    auto best_move_ = Move{};
    auto best_eval_ = -1000.0;

    for (const auto &move : movegen::Legal(_board)) {
        auto eval = Rollout(_board);
        if (eval > best_eval_) {
            best_eval_ = eval;
            best_move_ = move;
        }
    }

    return Result{0, 0, {best_move_}};
}
}   // namespace codbrain