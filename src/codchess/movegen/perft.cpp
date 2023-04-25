#include <codchess/perft.hpp>

#include <codchess/bitboard.hpp>
#include <codchess/move.hpp>
#include <codchess/movegen.hpp>

namespace codchess::perft {
int
Perft(Board &board, const int depth) noexcept {
    if (depth == 0) {
        return 1;
    }

    int nodes{0};
    for (const Move &move : movegen::PseudoLegal(board)) {
        const auto board_info = board.GetStateBackup();

        board.Make(move);

        const auto king_sq =
            bitboard::BitScanForward(board.pieces(board.inactive(), KING));
        if (!board.IsSquareAttacked(king_sq, board.active())) {
            nodes += Perft(board, depth - 1);
        }

        board.Unmake(move, board_info);
    }

    return nodes;
}
}   // namespace codchess::perft