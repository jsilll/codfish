#include "movepicker.hpp"

#include "eval.hpp"

#include "../movegen/bitboard.hpp"
#include "../movegen/board.hpp"
#include "../movegen/move.hpp"
#include "../movegen/movelist.hpp"
#include "../movegen/movegen.hpp"

#include <cstring>
#include <climits>

#define MIN_EVAL (INT_MIN + 1)

void MovePicker::setDepth(int depth)
{
    if (depth <= 0)
    {
        throw std::invalid_argument("Depth argument must be positive integer.");
    }

    _max_depth = depth;
}

int MovePicker::getDepth() const
{
    return _max_depth;
}

void MovePicker::addToKillerMoves(Move const &move)
{
    // Killer Move Heuristic
    if (move.getEncoded() != _killer_moves[0][_current_depth])
    {
        _killer_moves[1][_current_depth] = _killer_moves[0][_current_depth];
    }
    _killer_moves[0][_current_depth] = move.getEncoded();
}

void MovePicker::addToHistoryMoves(Move const &move)
{
    // History Move Heuristic
    _history_moves[_board.getSideToMove()][move.getPiece()][move.getToSquare()] += _current_depth;
}

void MovePicker::addToPrincipalVariation(Move const &move)
{
    // Write Principal Variation Move
    _pv_table[_current_depth][_current_depth] = move.getEncoded();

    // Copy moves from deeper depth into current depths line
    memcpy(&_pv_table[_current_depth][_current_depth + 1], &_pv_table[_current_depth + 1][_current_depth + 1], (unsigned long)_pv_length[_current_depth + 1] * sizeof(int));

    // Adjust Principal Variation Length
    _pv_length[_current_depth] = _pv_length[_current_depth + 1];
}

int MovePicker::score(const Move &move) const
{
    // clang-format off
    static const int MVV_LVA[6][6] = {
        {10105, 10205, 10305, 10405, 10505, 10605}, 
        {10104, 10204, 10304, 10404, 10504, 10604},
        {10103, 10203, 10303, 10403, 10503, 10603},
        {10102, 10202, 10302, 10402, 10502, 10602},
        {10101, 10201, 10301, 10401, 10501, 10601},
        {10100, 10200, 10300, 10400, 10500, 10600}
    };
    // clang-format on

    if (move.isCapture())
    {
        return MVV_LVA[move.getPiece()][move.getCapturedPiece()];
    }

    if (_killer_moves[0][_current_depth] == move.getEncoded())
    {
        return 9000;
    }

    if (_killer_moves[1][_current_depth] == move.getEncoded())
    {
        return 8000;
    }

    // TODO: can be improved after adding unmake move function
    return _history_moves[_current_depth % 2 ? !_board.getSideToMove() : _board.getSideToMove()][move.getPiece()][move.getToSquare()];
}

MovePicker::SearchResult MovePicker::findBestMove()
{
    memset(_history_moves, 0, sizeof(_history_moves));
    memset(_killer_moves, 0, sizeof(_killer_moves));
    memset(_killer_moves, 0, sizeof(_killer_moves));
    memset(_pv_table, 0, sizeof(_pv_table));

    // Iterative Deepening
    int alpha = 0;
    for (int depth = 1; depth <= _max_depth; depth++)
    {
        _current_nodes = 0;
        _current_depth = 0;
        alpha = search(depth);
    }

    // Search Result
    SearchResult res = SearchResult{alpha, _current_nodes, _pv_length[0]};
    for (int i = 0; i < res.pv_length; i++)
    {
        res.pv[i] = _pv_table[0][i];
    }

    return res;
}

int MovePicker::search(int depth)
{
    int alpha = MIN_EVAL;

    Move best_move = Move();
    MoveList moves = movegen::generatePseudoLegalMoves(_board);
    moves.sort(_move_more_than_key);
    for (const Move &move : moves)
    {
        Board backup = _board;
        backup.makeMove(move);
        int king_sq = bitboard::bitScanForward(backup.getPieces(backup.getOpponent(), KING));
        int attacker_side = backup.getSideToMove();
        if (!backup.isSquareAttacked(king_sq, attacker_side))
        {
            _current_depth++;
            int score = -negamax(MIN_EVAL, -alpha, depth - 1, backup);
            _current_depth--;
            if (score > alpha)
            {
                // History Move Heuristic
                if (!move.isCapture())
                {
                    addToHistoryMoves(move);
                }

                alpha = score;
                best_move = move;
            }
        }
    }

    addToPrincipalVariation(best_move);

    return alpha;
}

int MovePicker::negamax(int alpha, int beta, int depth, const Board &board)
{
    _current_nodes++;

    if (depth == 0)
    {
        _pv_length[_current_depth] = _current_depth;
        return quiescence(alpha, beta, -1, board);
    }

    Move best_move = Move();
    bool has_legal_moves = false;
    MoveList moves = movegen::generatePseudoLegalMoves(board);
    moves.sort(_move_more_than_key);
    for (const Move &move : moves)
    {
        Board backup = board;
        backup.makeMove(move);
        int king_sq = bitboard::bitScanForward(backup.getPieces(backup.getOpponent(), KING));
        if (!backup.isSquareAttacked(king_sq, backup.getSideToMove()))
        {
            has_legal_moves = true;
            _current_depth++;
            int score = -negamax(-beta, -alpha, depth - 1, backup);
            _current_depth--;
            if (score >= beta)
            {
                // Killer Move Heuristic
                if (!move.isCapture())
                {
                    addToKillerMoves(move);
                }

                return beta;
            }
            if (score > alpha)
            {
                // History Move Heuristic
                if (!move.isCapture())
                {
                    addToHistoryMoves(move);
                }

                alpha = score;
                best_move = move;
            }
        }
    }

    if (!has_legal_moves)
    {
        int king_sq = bitboard::bitScanForward(board.getPieces(board.getSideToMove(), KING));
        if (board.isSquareAttacked(king_sq, board.getOpponent()))
        {
            return MIN_EVAL + _current_depth;
        }

        return 0;
    }

    addToPrincipalVariation(best_move);

    return alpha;
}

int MovePicker::quiescence(int alpha, int beta, int depth, const Board &board)
{
    _current_nodes++;

    if (!movegen::hasLegalMoves(board))
    {
        int king_sq = bitboard::bitScanForward(board.getPieces(board.getSideToMove(), KING));
        if (board.isSquareAttacked(king_sq, board.getOpponent()))
        {
            return MIN_EVAL + _current_depth;
        }

        return 0;
    }

    int stand_pat = eval::eval(board);

    if (stand_pat >= beta)
    {
        return beta;
    }

    if (stand_pat > alpha)
    {
        alpha = stand_pat;
    }

    MoveList captures = movegen::generatePseudoLegalCaptures(board);
    captures.sort(_move_more_than_key);
    for (const Move &capture : captures)
    {
        Board backup = board;
        backup.makeMove(capture);
        int king_sq = bitboard::bitScanForward(backup.getPieces(backup.getOpponent(), KING));
        if (!backup.isSquareAttacked(king_sq, backup.getSideToMove()))
        {
            _current_depth++;
            int score = -quiescence(-beta, -alpha, depth - 1, backup);
            _current_depth--;
            if (score >= beta)
            {
                return beta;
            }
            if (score > alpha)
            {
                alpha = score;
            }
        }
    }

    return alpha;
}