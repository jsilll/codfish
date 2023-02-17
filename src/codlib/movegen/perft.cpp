#include <codlib/perft.hpp>

#include <codlib/bitboard.hpp>
#include <codlib/move.hpp>
#include <codlib/movegen.hpp>

namespace perft {
int perft(Board &board, const int depth)
{
  if (depth == 0) { return 1; }

  int nodes = 0;
  for (const Move &move : movegen::generate_pseudo_legal_moves(board)) {
    Board::StateBackup board_info = board.GetStateBackup();
      board.Make(move);
    Square king_sq = bitboard::bit_scan_forward(board.pieces(board.inactive(), KING));
    Color attacker_side = board.active();
    if (!board.IsSquareAttacked(king_sq, attacker_side)) { nodes += perft(board, depth - 1); }
      board.Unmake(move, board_info);
  }

  return nodes;
}
}// namespace perft