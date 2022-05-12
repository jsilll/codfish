#include "ai.hpp"

#include "utils.hpp"
#include "move.hpp"
#include "board.hpp"
#include "movelist.hpp"
#include "movegen.hpp"
#include "eval.hpp"

#include <limits>

Move AI::find_best_move()
{
    int best_val = std::numeric_limits<int>::min();
    Move best_move = Move(0, 0, 0, 0, 0, 0, 0, 0);

    for (Move move : Movegen::generatePseudoLegalMoves(_board))
    {
        Board backup = _board;
        backup.makeMove(move);
        int king_sq = Utils::bitScanForward(backup.getPieces(backup.getOpponent(), KING));
        int attacker_side = backup.getSideToMove();
        if (!backup.isSquareAttacked(king_sq, attacker_side))
        {
            int score = -search(std::numeric_limits<int>::min(), std::numeric_limits<int>::max(), 4, backup);

            if (score > best_val)
            {
                best_val = score;
                best_move = move;
            }
        }
    }

    return best_move;
}

int AI::search(int alpha, int beta, int depth, Board &board)
{
    if (depth == 0)
    {
        return Eval::eval(board);
    }

    int best_val = std::numeric_limits<int>::min();
    for (const Move &move : Movegen::generatePseudoLegalMoves(board))
    {
        Board backup = board;
        backup.makeMove(move);
        int king_sq = Utils::bitScanForward(backup.getPieces(backup.getOpponent(), KING));
        int attacker_side = backup.getSideToMove();
        if (!backup.isSquareAttacked(king_sq, attacker_side))
        {
            int score = -search(-beta, -alpha, depth - 1, backup);
            if (score > best_val)
            {
                best_val = score;
            }
            if (score >= beta)
            {
                break;
            }
            if (score > alpha)
            {
                alpha = score;
            }
            // if (alpha > beta)
            // {
            //     break;
            // }
        }
    }
    return best_val;
}