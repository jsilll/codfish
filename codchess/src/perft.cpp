#include <codchess/perft.hpp>

namespace codchess::perft {
/// @brief Auxiliary function for perft
/// @param board The board
/// @param depth The depth
/// @return The number of nodes
std::uint64_t
PerftUnmakeAux(Board &board, const std::uint32_t depth) noexcept {
    std::uint64_t nodes{0};
    const auto board_info = board.GetStateBackup();
    for (const auto move : movegen::PseudoLegal(board)) {
        board.Make(move);

        const auto king_sq =
            bitboard::BitScanForward(board.pieces(board.inactive(), KING));

        if (!board.IsSquareAttacked(king_sq, board.active())) {
            if (depth == 1) {
                nodes++;
            } else {
                nodes += PerftUnmakeAux(board, depth - 1);
            }
        }

        board.Unmake(move, board_info);
    }

    return nodes;
}

std::uint64_t
PerftUnmake(const Board &board, const std::uint32_t depth) noexcept {
    auto cboard = board;
    return PerftUnmakeAux(cboard, depth);
}

std::uint64_t
PerftCopyAux(Board &board, const std::uint32_t depth) noexcept {
    auto cboard = board;
    std::uint64_t nodes{0};
    for (const auto move : movegen::PseudoLegal(board)) {
        board.Make(move);

        const auto king_sq =
            bitboard::BitScanForward(board.pieces(board.inactive(), KING));

        if (!board.IsSquareAttacked(king_sq, board.active())) {
            if (depth == 1) {
                nodes++;
            } else {
                nodes += PerftCopyAux(board, depth - 1);
            }
        }

        board = cboard;
    }

    return nodes;
}

std::uint64_t
PerftCopy(const Board &board, const std::uint32_t depth) noexcept {
    auto cboard = board;
    return PerftCopyAux(cboard, depth);
}

}   // namespace codchess::perft