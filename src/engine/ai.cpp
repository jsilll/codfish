#include "ai.hpp"

#include "utils.hpp"
#include "bitboard.hpp"
#include "move.hpp"
#include "board.hpp"
#include "movelist.hpp"
#include "movegen.hpp"
#include "eval.hpp"

#include <climits>

#define MIN_EVAL (INT_MIN + 1)

void AI::setDepth(int depth)
{
    _depth = depth;
}

int AI::getDepth() const
{
    return _depth;
}

AI::SearchResult AI::findBestMove()
{
    _nodes = 0;

    int alpha = MIN_EVAL;
    Move best_move = Move();
    for (const Move &move : movegen::generatePseudoLegalMoves(_board))
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

int AI::search(int alpha, int beta, int depth, const Board &board)
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

int AI::quiescence(int alpha, int beta, int depth, const Board &board)
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

    for (const Move &capture : movegen::generatePseudoLegalCaptures(board))
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