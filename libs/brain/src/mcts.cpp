#include <brain/codbrain.hpp>

#include <algorithm>

using namespace codfish;

using core::Move;

namespace codfish::brain {
Mcts::RolloutResult
Mcts::Rollout() noexcept {
    auto moves = core::movegen::Legal(_board);
    if (moves.empty()) {
        return RolloutResult{_board.IsCheck() ? Outcome::Loss : Outcome::Draw,
                             _board.active()};
    } else if (_history.IsRepetition(_board.hash())) {
        return RolloutResult{Outcome::Draw, _board.active()};
    } else {
        std::shuffle(moves.begin(), moves.end(), _g);
        const auto backup = _board.GetStateBackup();
        Make(*moves.begin());
        const auto result = Rollout();
        Unmake(*moves.begin(), backup);
        return result;
    }
}

Result
Mcts::PickMove() noexcept {
    const auto side = _board.active();
    const auto backup = _board.GetStateBackup();
    const auto moves = core::movegen::Legal(_board);

    int best_wins{0};
    int best_draws{0};
    Move best_move{*moves.begin()};
    for (const auto &move : moves) {
        Make(move);
        int wins{0};
        int draws{0};
        for (int i = 0; i < 100000; ++i) {
            const auto result = Rollout();
            if (result.outcome == Outcome::Loss and result.last != side) {
                ++wins;
            } else if (result.outcome == Outcome::Draw) {
                ++draws;
            }
        }
        std::cout << "Move: " << move.ToString() << " Wins: " << wins << " Draws: " << draws << " in 1000\n";
        if (wins > best_wins) {
            best_wins = wins;
            best_move = move;
        } else if (wins == best_wins) {
            if (draws > best_draws) {
                best_draws = draws;
                best_move = move;
            }
        }
        Unmake(move, backup);
    }
    return Result{0, 0, {best_move}};
}
}   // namespace codfish::brain