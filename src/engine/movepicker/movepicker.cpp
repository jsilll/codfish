#include "movepicker.hpp"

#include "eval.hpp"

#include "../movegen/bitboard.hpp"
#include "../movegen/board.hpp"
#include "../movegen/move.hpp"
#include "../movegen/movelist.hpp"
#include "../movegen/movegen.hpp"

#include <climits>

#define MIN_EVAL (INT_MIN + 1)

// clang-format off
const int MVV_LVA[6][6] = {
    {105, 205, 305, 405, 505, 605}, 
    {104, 204, 304, 404, 504, 604},
    {103, 203, 303, 403, 503, 603},
    {102, 202, 302, 402, 502, 602},
    {101, 201, 301, 401, 501, 601},
    {100, 200, 300, 400, 500, 600}
};
// clang-format on

int score(const Move &move)
{

    if (move.isCapture())
    {
        return MVV_LVA[move.getPiece()][move.getCapturedPiece()];
    }

    return 0;
}

struct MoveMoreThanKey
{
    inline bool operator()(const Move &move1, const Move &move2)
    {
        return (score(move1) > score(move2));
    }
} moreThanKey;

void MovePicker::setDepth(int depth)
{
    _depth = depth;
}

int MovePicker::getDepth() const
{
    return _depth;
}

MovePicker::SearchResult MovePicker::findBestMove()
{
    _nodes = 0;

    int alpha = MIN_EVAL;
    Move best_move = Move();
    MoveList moves = movegen::generatePseudoLegalMoves(_board);
    moves.sort<MoveMoreThanKey>(moreThanKey);
    for (const Move &move : moves)
    {
        Board backup = _board;
        backup.makeMove(move);
        int king_sq = bitboard::bitScanForward(backup.getPieces(backup.getOpponent(), KING));
        int attacker_side = backup.getSideToMove();
        if (!backup.isSquareAttacked(king_sq, attacker_side))
        {
            int score = -search(MIN_EVAL, -alpha, _depth, backup);
            if (score > alpha)
            {
                alpha = score;
                best_move = move;
            }
        }
    }

    return SearchResult{alpha, best_move.getEncoded(), _nodes};
}

int MovePicker::search(int alpha, int beta, int depth, const Board &board)
{
    _nodes++;

    if (depth <= 0)
    {
        return quiescence(alpha, beta, depth, board);
    }

    bool has_legal_moves = false;
    for (const Move &move : movegen::generatePseudoLegalMoves(board))
    {
        Board backup = board;
        backup.makeMove(move);
        int king_sq = bitboard::bitScanForward(backup.getPieces(backup.getOpponent(), KING));
        if (!backup.isSquareAttacked(king_sq, backup.getSideToMove()))
        {
            has_legal_moves = true;
            int score = -search(-beta, -alpha, depth - 1, backup);
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

    if (!has_legal_moves)
    {
        int king_sq = bitboard::bitScanForward(board.getPieces(board.getSideToMove(), KING));
        if (board.isSquareAttacked(king_sq, board.getOpponent()))
        {
            return MIN_EVAL + _depth - depth;
        }

        return 0;
    }

    return alpha;
}

int MovePicker::quiescence(int alpha, int beta, int depth, const Board &board)
{
    _nodes++;

    if (!movegen::hasLegalMoves(board))
    {
        int king_sq = bitboard::bitScanForward(board.getPieces(board.getSideToMove(), KING));
        if (board.isSquareAttacked(king_sq, board.getOpponent()))
        {
            return MIN_EVAL + _depth - depth;
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
    captures.sort<MoveMoreThanKey>(moreThanKey);
    for (const Move &capture : captures)
    {
        Board backup = board;
        backup.makeMove(capture);
        int king_sq = bitboard::bitScanForward(backup.getPieces(backup.getOpponent(), KING));
        if (!backup.isSquareAttacked(king_sq, backup.getSideToMove()))
        {
            int score = -quiescence(-beta, -alpha, depth - 1, backup);
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