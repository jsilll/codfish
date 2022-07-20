#include <engine/movegen/perft.hpp>

#include <engine/movegen/bitboard.hpp>
#include <engine/movegen/board.hpp>
#include <engine/movegen/move.hpp>
#include <engine/movegen/movegen.hpp>

namespace perft
{
    unsigned long long perft(Board &board, int depth)
    {
        if (depth == 0)
        {
            return 1;
        }

        unsigned long long nodes = 0;
        for (const Move &move : movegen::generatePseudoLegalMoves(board))
        {
            Board::GameState board_info = board.getState();
            board.makeMove(move);
            int king_sq = bitboard::bitScanForward(board.getPieces(board.getOpponent(), KING));
            int attacker_side = board.getSideToMove();
            if (!board.isSquareAttacked(king_sq, attacker_side))
            {
                nodes += perft(board, depth - 1);
            }
            board.unmakeMove(move, board_info);
        }

        return nodes;
    }

} // namespace perft