#include <codbrain/codbrain.hpp>

#include <algorithm>
#include <random>

using namespace cod;

using chess::Move;

namespace cod::brain {
Evaluation
Rollout(chess::Board &board) noexcept {
    auto moves = chess::movegen::Legal(board);
    if (moves.empty()) {
        return board.IsCheck() ? -1 : 0;
    } else {
        std::random_device rd;
        std::mt19937 g(rd());
        std::shuffle(moves.begin(), moves.end(), g);
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

    for (const auto &move : chess::movegen::Legal(_board)) {
        auto eval = Rollout(_board);
        if (eval > best_eval_) {
            best_eval_ = eval;
            best_move_ = move;
        }
    }

    return Result{0, 0, {best_move_}};
}
}   // namespace cod::brain