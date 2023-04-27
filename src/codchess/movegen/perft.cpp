#include <codchess/perft.hpp>

#include <codchess/bitboard.hpp>
#include <codchess/move.hpp>
#include <codchess/movegen.hpp>

namespace codchess::perft {
/// @brief Auxiliary function for perft
/// @param board The board
/// @param depth The depth
/// @return The number of nodes
std::uint64_t
PerftAux(Board &board, std::uint64_t depth) noexcept {
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
                nodes += PerftAux(board, depth - 1);
            }
        }

        board.Unmake(move, board_info);
    }

    return nodes;
}

std::uint64_t
Perft(Board &board, std::uint64_t depth) noexcept {
    std::uint64_t nodes{0};

    const auto moves = movegen::PseudoLegal(board);
    const auto board_info = board.GetStateBackup();

#pragma omp parallel for reduction(+ : nodes) schedule(dynamic)
    for (auto it = moves.begin(); it != moves.end(); ++it) {
        auto cboard = board;

        cboard.Make(*it);

        const auto king_sq =
            bitboard::BitScanForward(cboard.pieces(cboard.inactive(), KING));

        if (!cboard.IsSquareAttacked(king_sq, cboard.active())) {
            if (depth == 1) {
                nodes++;
            } else {
                nodes += PerftAux(cboard, depth - 1);
            }
        }

        cboard.Unmake(*it, board_info);
    }

    return nodes;
}

}   // namespace codchess::perft