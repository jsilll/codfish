#include "ai.hpp"

#include "utils.hpp"
#include "move.hpp"
#include "board.hpp"
#include "movelist.hpp"
#include "movegen.hpp"
#include "eval.hpp"

#include <limits>

AI::SearchResult AI::find_best_move(int depth)
{
    Move bestmove = Move(0, 0, 0, 0, 0, 0, 0, 0);
    int alpha = std::numeric_limits<int>::min() + 1;
    for (const Move &move : Movegen::generatePseudoLegalMoves(_board))
    {
        Board backup = _board;
        backup.makeMove(move);
        int king_sq = Utils::bitScanForward(backup.getPieces(backup.getOpponent(), KING));
        int attacker_side = backup.getSideToMove();
        if (!backup.isSquareAttacked(king_sq, attacker_side))
        {
            int score = -search(std::numeric_limits<int>::min() + 1, -alpha, depth, backup);

            if (score > alpha)
            {
                alpha = score;
                bestmove = move;
            }
        }
    }

    return SearchResult{alpha, bestmove.getEncoded(), 10};
}

int AI::search(int alpha, int beta, int depth, Board &board)
{
    if (depth == 0)
    {
        return Eval::eval(board);
    }

    bool has_legal_moves = false;
    MoveList moves = Movegen::generatePseudoLegalMoves(board);
    for (const Move &move : moves)
    {
        Board backup = board;
        backup.makeMove(move);
        int king_sq = Utils::bitScanForward(backup.getPieces(backup.getOpponent(), KING));
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
        int king_sq = Utils::bitScanForward(board.getPieces(board.getOpponent(), KING));
        if (board.isSquareAttacked(king_sq, board.getOpponent()))
        {
            return std::numeric_limits<int>::min() + 2;
        }

        return 0;
    }

    return alpha;
}
