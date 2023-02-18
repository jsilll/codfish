#include <codchess/perft.hpp>

#include <codchess/bitboard.hpp>
#include <codchess/move.hpp>
#include <codchess/movegen.hpp>

namespace perft {
    int Perft(Board &board, int depth) {
        if (depth == 0) { return 1; }

        int nodes = 0;
        for (const Move &move: movegen::PseudoLegal(board)) {
            Board::StateBackup board_info = board.GetStateBackup();
            board.Make(move);
            Square king_sq = bitboard::BitScanForward(board.pieces(board.inactive(), KING));
            Color attacker_side = board.active();
            if (!board.IsSquareAttacked(king_sq, attacker_side)) { nodes += Perft(board, depth - 1); }
            board.Unmake(move, board_info);
        }

        return nodes;
    }
}// namespace perft