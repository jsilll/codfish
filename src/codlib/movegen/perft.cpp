#include "perft.hpp"

#include "codlib/bitboard.hpp"
#include "codlib/movegen.hpp"
#include "codlib/zobrist.hpp"
#include <codlib/move.hpp>

namespace perft
{
    int perft(Board &board, int depth)
    {
        if (depth == 0)
        {
            return 1;
        }

        int nodes = 0;
        for (const Move &move : movegen::generate_pseudo_legal_moves(board))
        {
            Board::GameState board_info = board.get_state();
            board.make(move);
            Square king_sq = bitboard::bit_scan_forward(board.get_pieces(board.get_opponent(), KING));
            Color attacker_side = board.get_side_to_move();
            if (!board.is_square_attacked(king_sq, attacker_side))
            {
                nodes += perft(board, depth - 1);
            }
            board.unmake(move, board_info);
        }

        return nodes;
    }
} // namespace perft