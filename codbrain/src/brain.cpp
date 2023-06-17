#include <codbrain/brain.hpp>

#include <algorithm>
#include <climits>
#include <cstring>

#include <codchess/bitboard.hpp>
#include <codchess/board.hpp>
#include <codchess/move.hpp>
#include <codchess/movegen.hpp>

#include <codbrain/eval.hpp>

#define R                2
#define REDUCTION_LIMIT  3
#define FULL_DEPTH_MOVES 4
#define WINDOW_EXPANSION 50
#define MIN_EVAL         (INT_MIN + 1)

using namespace codchess;

namespace codbrain {
std::int32_t
Brain::Score(const codchess::Move move) const noexcept {
    static constexpr int MVV_LVA[6][6] = {
        {10105, 10205, 10305, 10405, 10505, 10605},
        {10104, 10204, 10304, 10404, 10504, 10604},
        {10103, 10203, 10303, 10403, 10503, 10603},
        {10102, 10202, 10302, 10402, 10502, 10602},
        {10101, 10201, 10301, 10401, 10501, 10601},
        {10100, 10200, 10300, 10400, 10500, 10600}};

    if (_pv_table.PVMove(_current_depth) == move) {
        return 20000;
    } else if (move.IsCapture()) {
        return MVV_LVA[move.MovedPiece()][move.CapturedPiece()];
    } else if (_killer_table.First(_current_depth) == move) {
        return 9000;
    } else if (_killer_table.Second(_current_depth) == move) {
        return 8000;
    } else {
        return _hist_moves.Get(_board.active(), move);
    }
}

int
Brain::Search(int alpha, int beta, int depth) noexcept {
    _pv_table.SetDepth(depth);

    auto moves = movegen::PseudoLegal(_board);
    std::sort(moves.begin(), moves.end(),
              [this](const auto &m1, const auto &m2) noexcept {
                  return Score(m1) > Score(m2);
              });

    Move best{};
    const auto state = _board.GetStateBackup();
    for (const auto move : moves) {
        _board.Make(move);

        const auto king_sq =
            bitboard::BitScanForward(_board.pieces(_board.inactive(), KING));

        if (!_board.IsSquareAttacked(king_sq, _board.active())) {
            ++_current_depth;
            _hist_table.Push(state.hash);
            const auto score = -Negamax(-beta, -alpha, depth - 1);
            _hist_table.Pop();
            --_current_depth;

            if (score > alpha) {
                if (!move.IsCapture()) {
                    // History Move Heuristic
                    _hist_moves.Add(_board.active(), move, _current_depth);
                }

                best = move;
                alpha = score;
                _pv_table.Add(best, _current_depth);
            }
        }

        _board.Unmake(move, state);
    }

    return alpha;
}

std::int32_t
Brain::Negamax(int alpha, int beta, int depth) noexcept {
    ++_current_nodes;

    // Terminal Node
    if (_board.half_move_clock() == 100) {
        // Draw
        return 0;
    }

    // Terminal Node
    if (_hist_table.IsRepetition(_board.hash())) {
        // Three-Fold Draw
        return 0;
    }

    const auto res = _ttable.Read(_board.hash(), alpha, beta, depth);
    if (res.found) {
        _pv_table.Add(res.moves, _current_depth);
        return res.score;
    }

    // Forced Terminal Node
    if (depth <= 0) {
        return Quiescence(alpha, beta);
    }

    const auto state = _board.GetStateBackup();

    // Null Move Pruning
    if (depth >= 3) {
        // TODO: Zugzwang checking
        _board.SwitchActive();
        _board.en_passant_square(EMPTY_SQUARE);
        _current_depth += 2;

        const auto score = -Negamax(-beta, -beta + 1, depth - 1 - R);

        _current_depth -= 2;
        _board.SwitchActive();
        _board.SetStateBackup(state);

        if (score >= beta) {
            _ttable.Set(state.hash, depth, TTable::HASH_FLAG_BETA, beta,
                        _pv_table.PV(_current_depth));

            return beta;
        }
    }

    auto moves = movegen::PseudoLegal(_board);
    std::sort(moves.begin(), moves.end(),
              [this](const auto &m1, const auto &m2) noexcept {
                  return Score(m1) > Score(m2);
              });

    Move best{};
    int n_moves_searched{0};
    bool has_legal_moves = false;
    int alpha_cutoff = TTable::HASH_FLAG_ALPHA;
    for (const auto move : moves) {
        _board.Make(move);

        const auto king_sq =
            bitboard::BitScanForward(_board.pieces(_board.inactive(), KING));

        if (!_board.IsSquareAttacked(king_sq, _board.active())) {
            has_legal_moves = true;

            int score{0};
            // First move, then use Full Window Search
            if (n_moves_searched == 0) {
                ++_current_depth;
                _hist_table.Push(state.hash);
                score = -Negamax(-beta, -alpha, depth - 1);
                _hist_table.Pop();
                --_current_depth;
            } else {
                // For all the others moves, we assume they are worse moves than
                // the first one, so let's try to use Null Window Search first
                if (n_moves_searched >= FULL_DEPTH_MOVES and
                    depth >= REDUCTION_LIMIT and CanLMR(move)) {
                    // Perform a Null Window Search
                    ++_current_depth;
                    _hist_table.Push(state.hash);
                    score = -Negamax(-(alpha + 1), -alpha, depth - 2);
                    _hist_table.Pop();
                    --_current_depth;
                } else {
                    // Hack to ensure that Full Depth Search is done
                    score = alpha + 1;
                }

                // If this move failed to prove to be bad, re-search with normal
                // bounds
                if (score > alpha) {
                    ++_current_depth;
                    _hist_table.Push(state.hash);
                    score = -Negamax(-(alpha + 1), -alpha, depth - 1);
                    _hist_table.Pop();
                    --_current_depth;

                    if ((score > alpha) and (score < beta)) {
                        ++_current_depth;
                        _hist_table.Push(state.hash);
                        score = -Negamax(-beta, -alpha, depth - 1);
                        _hist_table.Pop();
                        --_current_depth;
                    }
                }
            }

            if (score >= beta) {
                // Killer Move Heuristic
                if (!move.IsCapture()) {
                    _killer_table.Push(move, _current_depth);
                }

                _ttable.Set(state.hash, depth, TTable::HASH_FLAG_BETA, beta,
                            _pv_table.PV(_current_depth));

                _board.Unmake(move, state);
                return beta;
            } else if (score > alpha) {
                // History Move Heuristic
                if (!move.IsCapture()) {
                    _hist_moves.Add(_board.active(), move, _current_depth);
                }

                best = move;
                alpha = score;
                alpha_cutoff = TTable::HASH_FLAG_SCORE;
                _pv_table.Add(best, _current_depth);
            }

            ++n_moves_searched;
        }

        _board.Unmake(move, state);
    }

    // Terminal Node
    if (!has_legal_moves) {
        // Check Mate
        const auto king_sq =
            bitboard::BitScanForward(_board.pieces(_board.active(), KING));

        if (_board.IsSquareAttacked(king_sq, _board.inactive())) {
            _ttable.Set(state.hash, depth, TTable::HASH_FLAG_SCORE,
                        MIN_EVAL + _current_depth,
                        _pv_table.PV(_current_depth));

            return MIN_EVAL + _current_depth;
        }

        // Stale Mate
        _ttable.Set(state.hash, depth, TTable::HASH_FLAG_SCORE, 0,
                    _pv_table.PV(_current_depth));

        return 0;
    }

    _ttable.Set(state.hash, depth, alpha_cutoff, alpha,
                _pv_table.PV(_current_depth));

    return alpha;
}

int
Brain::Quiescence(int alpha, int beta) noexcept {
    ++_current_nodes;

    if (_board.half_move_clock() == 100) {
        _ttable.Set(_board.hash(), 0, TTable::HASH_FLAG_SCORE, 0,
                    _pv_table.PV(_current_depth));
        return 0;
    }

    if (!movegen::HasLegal(_board)) {
        const auto king_sq =
            bitboard::BitScanForward(_board.pieces(_board.active(), KING));

        if (_board.IsSquareAttacked(king_sq, _board.inactive())) {
            _ttable.Set(_board.hash(), 0, TTable::HASH_FLAG_SCORE,
                        MIN_EVAL + _current_depth,
                        _pv_table.PV(_current_depth));
            return MIN_EVAL + _current_depth;
        }

        _ttable.Set(_board.hash(), 0, TTable::HASH_FLAG_SCORE, 0,
                    _pv_table.PV(_current_depth));

        return 0;
    }

    const auto stand_pat = eval::Static(_board);
    auto alpha_cutoff = TTable::HASH_FLAG_ALPHA;
    if (stand_pat >= beta) {
        _ttable.Set(_board.hash(), 0, TTable::HASH_FLAG_BETA, beta,
                    _pv_table.PV(_current_depth));
        return beta;
    } else if (stand_pat > alpha) {
        alpha_cutoff = TTable::HASH_FLAG_SCORE;
        alpha = stand_pat;
    }

    auto captures = movegen::PseudoLegalCaptures(_board);
    std::sort(captures.begin(), captures.end(),
              [this](const auto &m1, const auto &m2) noexcept {
                  return Score(m1) > Score(m2);
              });

    const auto hash = _board.hash();
    const auto state = _board.GetStateBackup();
    for (const auto capture : captures) {
        _board.Make(capture);

        const auto king_sq =
            bitboard::BitScanForward(_board.pieces(_board.inactive(), KING));

        if (!_board.IsSquareAttacked(king_sq, _board.active())) {
            ++_current_depth;
            const auto score = -Quiescence(-beta, -alpha);
            --_current_depth;

            if (score >= beta) {
                _ttable.Set(hash, 0, TTable::HASH_FLAG_BETA, beta,
                            _pv_table.PV(_current_depth));

                _board.Unmake(capture, state);
                return beta;
            } else if (score > alpha) {
                alpha = score;
                alpha_cutoff = TTable::HASH_FLAG_SCORE;
            }
        }

        _board.Unmake(capture, state);
    }

    _ttable.Set(_board.hash(), 0, alpha_cutoff, alpha,
                _pv_table.PV(_current_depth));

    return alpha;
}

Brain::Result
Brain::FindBestMove() noexcept {
    int alpha{MIN_EVAL}, beta{-MIN_EVAL};

    ClearMoveTables();

    // Iterative Deepening
    for (std::uint32_t depth = 1; depth <= _max_depth; ++depth) {
        ClearCounters();

        const auto score = Search(alpha, beta, depth);

        // Aspiration Window
        if (score <= alpha or score >= beta) {
            // We fall outside the window, so the next search
            // iteration is going to have a full width window and same depth
            --depth;
            alpha = MIN_EVAL;
            beta = -MIN_EVAL;

            continue;
        }

        alpha = score - WINDOW_EXPANSION;
        beta = score + WINDOW_EXPANSION;
    }

    return {alpha, _current_nodes, _pv_table.PV()};
}
}   // namespace codbrain