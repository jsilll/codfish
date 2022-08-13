#include <engine/movegen/perft.hpp>

#include <engine/bitboard.hpp>
#include <engine/board.hpp>
#include <engine/move.hpp>
#include <engine/movegen/movegen.hpp>
#include <engine/zobrist.hpp>

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
            board.make_move(move);
            if (board.calculate_hash_key() != zobrist::generate_hash_key(board))
            {
                std::cout << "make: " << board.get_fen() << "   " << move.get_uci() << std::endl;
            }
            int king_sq = bitboard::bit_scan_forward(board.get_pieces(board.get_opponent(), KING));
            int attacker_side = board.get_side_to_move();
            if (!board.is_square_attacked(king_sq, attacker_side))
            {
                nodes += perft(board, depth - 1);
            }
            board.unmake_move(move, board_info);
            if (board.calculate_hash_key() != zobrist::generate_hash_key(board))
            {
                std::cout << "unmake: " << board.get_fen() << "   " << move.get_uci() << std::endl;
            }
        }

        return nodes;
    }

} // namespace perft