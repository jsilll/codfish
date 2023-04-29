#include <codbrain/codbrain.hpp>

#include <algorithm>
#include <climits>
#include <cstring>

#include <codchess/bitboard.hpp>
#include <codchess/board.hpp>
#include <codchess/move.hpp>
#include <codchess/movegen.hpp>

#include "eval.hpp"

#define R                2
#define REDUCTION_LIMIT  3
#define FULL_DEPTH_MOVES 4
#define WINDOW_EXPANSION 50
#define MIN_EVAL         (INT_MIN + 1)

using namespace codchess;

namespace codbrain {
static bool
can_lmr(const Move move) {
    // Move can't be a capture nor a promotion for LMR to happen
    /* TODO: missing check moves and in check moves */
    return !move.IsCapture() && !move.IsPromotion();
}

int
MovePicker::Search(int alpha, int beta, int depth) {
    auto moves = movegen::PseudoLegal(_board);
    // std::sort(moves.begin(), moves.end(), _move_more_than_key);

    Move best_move = Move();
    const auto state = _board.GetStateBackup();
    for (const auto move : moves) {
        _board.Make(move);

        Color attacker_side = _board.active();
        Square king_sq =
            bitboard::BitScanForward(_board.pieces(_board.inactive(), KING));
        if (!_board.IsSquareAttacked(king_sq, attacker_side)) {
            _current_depth++;
            _hist_table.Push(state.hash_key);
            int score = -Negamax(-beta, -alpha, depth - 1);
            _hist_table.Pop();
            _current_depth--;
            if (score > alpha) {
                // History Move Heuristic
                if (!move.IsCapture()) {
                    AddToHistoryMoves(move);
                }

                alpha = score;
                best_move = move;
                _pv_table.add(best_move, _current_depth);
            }
        }

        _board.Unmake(move, state);
    }

    return alpha;
}

std::int32_t
MovePicker::Negamax(int alpha, int beta, int depth) noexcept {
    _current_nodes++;

    // Terminal Node
    if (_board.half_move_clock() == 100) {
        // Draw
        return 0;
    }

    // Terminal Node
    if (_hist_table.IsRepetition(_board.hash_key())) {
        // Three-Fold Draw
        return 0;
    }

    TTable::TTOutput hash_read =
        _ttable.read_hash(_board.hash_key(), alpha, beta, depth);
    if (hash_read.found) {
        _pv_table.add_pv_from_depth(hash_read.moves, _current_depth);
        return hash_read.score;
    }

    // Forced Terminal Node
    if (depth <= 0) {
        _pv_table.set_length(_current_depth);
        return Quiescence(alpha, beta);
    }

    const auto state = _board.GetStateBackup();

    // Null Move Pruning
    // TODO: Zugzwang checking
    if (depth >= 3) {
        _board.SwitchActive();
        _board.en_passant_square(EMPTY_SQUARE);
        _current_depth += 2;
        int score = -Negamax(-beta, -beta + 1, depth - 1 - R);
        _current_depth -= 2;
        _board.SetStateBackup(state);
        _board.SwitchActive();
        if (score >= beta) {
            _ttable.set_entry(state.hash_key, depth, TTable::HASH_FLAG_BETA,
                              beta,
                              _pv_table.get_pv_from_depth(_current_depth));
            return beta;
        }
    }

    auto moves = movegen::PseudoLegal(_board);
    // std::sort(moves.begin(), moves.end(), _move_more_than_key);

    Move best_move = Move();
    int n_moves_searched = 0;
    bool has_legal_moves = false;
    int alpha_cutoff = TTable::HASH_FLAG_ALPHA;
    for (const auto move : moves) {
        _board.Make(move);

        Square king_sq =
            bitboard::BitScanForward(_board.pieces(_board.inactive(), KING));
        if (!_board.IsSquareAttacked(king_sq, _board.active())) {
            has_legal_moves = true;

            int score;
            // First move, then use Full Window Search
            if (n_moves_searched == 0) {
                _current_depth++;
                _hist_table.Push(state.hash_key);
                score = -Negamax(-beta, -alpha, depth - 1);
                _hist_table.Pop();
                _current_depth--;
            } else {
                // For all the others moves, we assume they are worse moves than
                // the first one, so let's try to use Null Window Search first
                if (n_moves_searched >= FULL_DEPTH_MOVES &&
                    depth >= REDUCTION_LIMIT && can_lmr(move)) {
                    // Perform a Null Window Search
                    _current_depth++;
                    _hist_table.Push(state.hash_key);
                    score = -Negamax(-(alpha + 1), -alpha, depth - 2);
                    _hist_table.Pop();
                    _current_depth--;
                } else {
                    // Hack to ensure that Full Depth Search is done
                    score = alpha + 1;
                }

                // If this move failed to prove to be bad, re-search with normal
                // bounds
                if (score > alpha) {
                    _current_depth++;
                    _hist_table.Push(state.hash_key);
                    score = -Negamax(-(alpha + 1), -alpha, depth - 1);
                    _hist_table.Pop();
                    _current_depth--;

                    if ((score > alpha) && (score < beta)) {
                        _current_depth++;
                        _hist_table.Push(state.hash_key);
                        score = -Negamax(-beta, -alpha, depth - 1);
                        _hist_table.Pop();
                        _current_depth--;
                    }
                }
            }

            if (score >= beta) {
                // Killer Move Heuristic
                if (!move.IsCapture()) {
                    AddToKillerMoves(move);
                }

                _board.Unmake(move, state);

                _ttable.set_entry(state.hash_key, depth, TTable::HASH_FLAG_BETA,
                                  beta,
                                  _pv_table.get_pv_from_depth(_current_depth));
                return beta;
            } else if (score > alpha) {

                // History Move Heuristic
                if (!move.IsCapture()) {
                    AddToHistoryMoves(move);
                }

                alpha = score;
                alpha_cutoff = TTable::HASH_FLAG_SCORE;
                best_move = move;
                _pv_table.add(best_move, _current_depth);
            }

            n_moves_searched++;
        }

        _board.Unmake(move, state);
    }

    // Terminal Node
    if (!has_legal_moves) {
        // Check Mate
        Square king_sq =
            bitboard::BitScanForward(_board.pieces(_board.active(), KING));
        if (_board.IsSquareAttacked(king_sq, _board.inactive())) {
            _ttable.set_entry(state.hash_key, depth, TTable::HASH_FLAG_SCORE,
                              MIN_EVAL + _current_depth,
                              _pv_table.get_pv_from_depth(_current_depth));
            return MIN_EVAL + _current_depth;
        }

        // Stale Mate
        _ttable.set_entry(state.hash_key, depth, TTable::HASH_FLAG_SCORE, 0,
                          _pv_table.get_pv_from_depth(_current_depth));
        return 0;
    }

    _ttable.set_entry(state.hash_key, depth, alpha_cutoff, alpha,
                      _pv_table.get_pv_from_depth(_current_depth));
    return alpha;
}

int
MovePicker::Quiescence(int alpha, int beta) {
    _current_nodes++;

    if (_board.half_move_clock() == 100) {
        _ttable.set_entry(_board.hash_key(), 0, TTable::HASH_FLAG_SCORE, 0,
                          _pv_table.get_pv_from_depth(_current_depth));
        return 0;
    }

    if (!movegen::HasLegal(_board)) {
        Square king_sq =
            bitboard::BitScanForward(_board.pieces(_board.active(), KING));
        if (_board.IsSquareAttacked(king_sq, _board.inactive())) {
            _ttable.set_entry(_board.hash_key(), 0, TTable::HASH_FLAG_SCORE,
                              MIN_EVAL + _current_depth,
                              _pv_table.get_pv_from_depth(_current_depth));
            return MIN_EVAL + _current_depth;
        }

        _ttable.set_entry(_board.hash_key(), 0, TTable::HASH_FLAG_SCORE, 0,
                          _pv_table.get_pv_from_depth(_current_depth));
        return 0;
    }

    int alpha_cutoff = TTable::HASH_FLAG_ALPHA;

    int stand_pat = eval::eval(_board);
    if (stand_pat >= beta) {
        _ttable.set_entry(_board.hash_key(), 0, TTable::HASH_FLAG_BETA, beta,
                          _pv_table.get_pv_from_depth(_current_depth));
        return beta;
    } else if (stand_pat > alpha) {

        alpha_cutoff = TTable::HASH_FLAG_SCORE;
        alpha = stand_pat;
    }

    auto captures = movegen::PseudoLegalCaptures(_board);
    // std::sort(captures.begin(), captures.end(), _move_more_than_key);

    const auto hash_key = _board.hash_key();
    const auto state = _board.GetStateBackup();
    for (const Move &capture : captures) {
        _board.Make(capture);

        Square king_sq =
            bitboard::BitScanForward(_board.pieces(_board.inactive(), KING));
        if (!_board.IsSquareAttacked(king_sq, _board.active())) {
            _current_depth++;
            int score = -Quiescence(-beta, -alpha);
            _current_depth--;

            if (score >= beta) {
                _board.Unmake(capture, state);
                _ttable.set_entry(hash_key, 0, TTable::HASH_FLAG_BETA, beta,
                                  _pv_table.get_pv_from_depth(_current_depth));
                return beta;
            } else if (score > alpha) {

                alpha_cutoff = TTable::HASH_FLAG_SCORE;
                alpha = score;
            }
        }

        _board.Unmake(capture, state);
    }

    _ttable.set_entry(_board.hash_key(), 0, alpha_cutoff, alpha,
                      _pv_table.get_pv_from_depth(_current_depth));
    return alpha;
}

MovePicker::SearchResult
MovePicker::FindBestMove() noexcept {
    int alpha = MIN_EVAL;
    int beta = -MIN_EVAL;

    ClearMoveTables();

    // Iterative Deepening
    for (std::uint32_t depth = 1; depth <= _max_depth; depth++) {
        ClearCounters();
        int score = Search(alpha, beta, depth);

        // Aspiration Window
        if ((score <= alpha) || (score >= beta)) {
            // We fall outside the window, so the next search
            // iteration is going to have a full width window and same depth
            alpha = MIN_EVAL;
            beta = -MIN_EVAL;

            depth--;
            continue;
        }

        alpha = score - WINDOW_EXPANSION;
        beta = score + WINDOW_EXPANSION;
    }

    auto result = SearchResult{alpha, _current_nodes, _pv_table.get_pv()};
    return result;
}

}   // namespace codbrain