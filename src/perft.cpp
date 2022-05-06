#include "perft.hpp"

#include "utils.hpp"
#include "move.hpp"
#include "movelist.hpp"
#include "board.hpp"
#include "movegen.hpp"

unsigned long long Perft::perft(const Board &board, int depth)
{
    if (depth == 0)
    {
        return 1;
    }

    unsigned long long nodes = 0;
    for (Move const &move : Movegen::generatePseudoLegalMoves(board))
    {
        Board backup = board;
        backup.makeMove(move);
        int king_sq = Utils::bitScanForward(backup.getPieces(backup.getOpponent(), KING));
        int attacker_side = backup.getSideToMove();
        if (!backup.isSquareAttacked(king_sq, attacker_side))
        {
            nodes += perft(backup, depth - 1);
        }
    }

    return nodes;
}