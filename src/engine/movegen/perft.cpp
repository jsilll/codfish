#include "perft.hpp"

#include "bitboard.hpp"
#include "board.hpp"
#include "move.hpp"
#include "movelist.hpp"
#include "movegen.hpp"

namespace perft
{
    unsigned long long perft(const Board &board, int depth)
    {
        if (depth == 0)
        {
            return 1;
        }

        unsigned long long nodes = 0;
        for (Move const &move : movegen::generatePseudoLegalMoves(board))
        {
            Board backup = board;
            backup.makeMove(move);
            int king_sq = bitboard::bitScanForward(backup.getPieces(backup.getOpponent(), KING));
            int attacker_side = backup.getSideToMove();
            if (!backup.isSquareAttacked(king_sq, attacker_side))
            {
                nodes += perft(backup, depth - 1);
            }
        }

        return nodes;
    }
} // namespace perft