#include <codchess/perft.hpp>

#include <codchess/bitboard.hpp>
#include <codchess/move.hpp>
#include <codchess/movegen.hpp>

namespace codchess::perft {
std::uint32_t
Perft(Board &board, std::uint32_t depth) noexcept {
    std::uint32_t nodes{0};
    for (const Move &move : movegen::PseudoLegal(board)) {
        const auto board_info = board.GetStateBackup();

        board.Make(move);

        const auto king_sq =
            bitboard::BitScanForward(board.pieces(board.inactive(), KING));

        if (!board.IsSquareAttacked(king_sq, board.active())) {
            if (depth == 1) {
                nodes++;
            } else {
                nodes += Perft(board, depth - 1);
            }
        }

        board.Unmake(move, board_info);
    }

    return nodes;
}
}   // namespace codchess::perft