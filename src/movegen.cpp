#include "movegen.hpp"

#include "utils.hpp"
#include "attacks.hpp"
#include "tables.hpp"
#include "board.hpp"
#include "move.hpp"
#include "movelist.hpp"

namespace movegen
{
  inline void generateCastlingMoves(MoveList &move_list, const Board &board, int opponent, int castle_b_sq, int castle_c_sq, int castle_d_sq, int castle_e_sq, int castle_f_sq, int castle_g_sq, int castle_queen_mask, int castle_king_mask);

  inline void generatePawnDoublePushes(MoveList &move_list, U64 pawn_double_pushes, int pawn_double_push_offset);
  inline void generatePawnSinglePushWithPromotion(MoveList &move_list, U64 pawn_single_pushes, int pawn_single_push_offset);
  inline void generatePawnSinglePushNoPromotion(MoveList &move_list, U64 pawn_single_pushes, int pawn_single_push_offset);
  inline void generatePawnCapturesWithPromotion(MoveList &move_list, int to_move, U64 to_move_pawns, U64 opponent_occupancies);
  inline void generatePawnCapturesNoPromotion(MoveList &move_list, int to_move, U64 to_move_pawns, U64 opponent_occupancies);
  inline void generateEnPassantCapture(MoveList &move_list, U64 to_move_pawns, int en_passant_square, int opponent);

  inline void generateKnightMoves(MoveList &move_list, U64 to_move_knights, U64 to_move_occupancies, U64 opponent_occupancies);
  inline void generateKingMoves(MoveList &move_list, U64 to_move_king, U64 to_move_occupancies, U64 opponent_occupancies);

  inline void generateBishopMoves(MoveList &move_list, U64 to_move_bishops, U64 to_move_occupancies, U64 opponent_occupancies, U64 both_occupancies);
  inline void generateRookMoves(MoveList &move_list, U64 to_move_rooks, U64 to_move_occupancies, U64 opponent_occupancies, U64 both_occupancies);
  inline void generateQueenMoves(MoveList &move_list, U64 to_move_queens, U64 to_move_occupancies, U64 opponent_occupancies, U64 both_occupancies);

  MoveList generatePseudoLegalMoves(const Board &board)
  {
    int to_move = board.getSideToMove();
    int opponent = board.getOpponent();

    int castle_b_sq = to_move == WHITE ? B1 : B8;
    int castle_c_sq = to_move == WHITE ? C1 : C8;
    int castle_d_sq = to_move == WHITE ? D1 : D8;
    int castle_e_sq = to_move == WHITE ? E1 : E8;
    int castle_f_sq = to_move == WHITE ? F1 : F8;
    int castle_g_sq = to_move == WHITE ? G1 : G8;

    int castle_king_mask = to_move == WHITE ? CASTLE_KING_WHITE : CASTLE_KING_BLACK;
    int castle_queen_mask = to_move == WHITE ? CASTLE_QUEEN_WHITE : CASTLE_QUEEN_BLACK;

    int pawn_double_push_offset = to_move == WHITE ? -16 : 16;
    int pawn_single_push_offset = to_move == WHITE ? pawn_double_push_offset / 2 : pawn_double_push_offset / 2;

    U64 to_move_occupancies = board.getOccupancies(to_move);
    U64 opponent_occupancies = board.getOccupancies(opponent);
    U64 both_occupancies = board.getOccupancies(BOTH);

    U64 to_move_pawns = board.getPieces(to_move, PAWN);
    U64 pawn_double_pushes = to_move == WHITE ? attacks::maskWhitePawnDoublePushes(to_move_pawns, ~both_occupancies) : attacks::maskBlackPawnDoublePushes(to_move_pawns, ~both_occupancies);
    U64 pawn_single_pushes = to_move == WHITE ? attacks::maskWhitePawnSinglePushes(to_move_pawns, ~both_occupancies) : attacks::maskBlackPawnSinglePushes(to_move_pawns, ~both_occupancies);

    MoveList move_list = MoveList();

    // Pawn Moves
    generatePawnCapturesWithPromotion(move_list, to_move, to_move_pawns, opponent_occupancies);
    generatePawnCapturesNoPromotion(move_list, to_move, to_move_pawns, opponent_occupancies);
    generateEnPassantCapture(move_list, to_move_pawns, board.getEnPassantSquare(), opponent);
    generatePawnSinglePushWithPromotion(move_list, pawn_single_pushes, pawn_single_push_offset);
    generatePawnDoublePushes(move_list, pawn_double_pushes, pawn_double_push_offset);
    generatePawnSinglePushNoPromotion(move_list, pawn_single_pushes, pawn_single_push_offset);

    // Leaper Pieces
    generateKnightMoves(move_list, board.getPieces(to_move, KNIGHT), to_move_occupancies, opponent_occupancies);
    generateKingMoves(move_list, board.getPieces(to_move, KING), to_move_occupancies, opponent_occupancies);

    // Slider Pieces
    generateBishopMoves(move_list, board.getPieces(to_move, BISHOP), to_move_occupancies, opponent_occupancies, both_occupancies);
    generateRookMoves(move_list, board.getPieces(to_move, ROOK), to_move_occupancies, opponent_occupancies, both_occupancies);
    generateQueenMoves(move_list, board.getPieces(to_move, QUEEN), to_move_occupancies, opponent_occupancies, both_occupancies);

    // Castling Moves
    generateCastlingMoves(move_list, board, opponent, castle_b_sq, castle_c_sq, castle_d_sq, castle_e_sq, castle_f_sq, castle_g_sq, castle_queen_mask, castle_king_mask);

    return move_list;
  }

  MoveList generateLegalMoves(const Board &board)
  {
    MoveList legal_moves;
    for (Move const &move : movegen::generatePseudoLegalMoves(board))
    {
      Board backup = board;
      backup.makeMove(move);
      int king_sq = utils::bitScanForward(backup.getPieces(board.getSideToMove(), KING));
      int attacker_side = backup.getSideToMove();
      if (!backup.isSquareAttacked(king_sq, attacker_side))
      {
        legal_moves.push_back(move);
      }
    }
    return legal_moves;
  }

  void generateCastlingMoves(MoveList &move_list, const Board &board, int opponent, int castle_b_sq, int castle_c_sq, int castle_d_sq, int castle_e_sq, int castle_f_sq, int castle_g_sq, int castle_queen_mask, int castle_king_mask)
  {
    if (!board.isSquareAttacked(castle_e_sq, opponent))
    {
      if ((board.getCastlingRights() & castle_king_mask) && !utils::getBit(board.getOccupancies(BOTH), castle_f_sq) && !utils::getBit(board.getOccupancies(BOTH), castle_g_sq))
      {
        if (!board.isSquareAttacked(castle_f_sq, opponent && !board.isSquareAttacked(castle_g_sq, opponent)))
        {
          move_list.push_back(Move(castle_e_sq, castle_g_sq, KING, 0, false, false, false, true));
        }
      }
      if ((board.getCastlingRights() & castle_queen_mask) && !utils::getBit(board.getOccupancies(BOTH), castle_d_sq) && !utils::getBit(board.getOccupancies(BOTH), castle_c_sq) && !utils::getBit(board.getOccupancies(BOTH), castle_b_sq))
      {
        if (!board.isSquareAttacked(castle_d_sq, opponent) && !board.isSquareAttacked(castle_c_sq, opponent))
        {
          move_list.push_back(Move(castle_e_sq, castle_c_sq, KING, 0, false, false, false, true));
        }
      }
    }
  }

  void generatePawnDoublePushes(MoveList &move_list, U64 pawn_double_pushes, int pawn_double_push_offset)
  {
    while (pawn_double_pushes)
    {
      int to_square = utils::bitScanForward(pawn_double_pushes);
      int from_square = to_square + pawn_double_push_offset;
      move_list.push_back(Move(from_square, to_square, PAWN, 0, false, true, false, false));
      utils::popLastBit(pawn_double_pushes);
    }
  }

  void generatePawnSinglePushWithPromotion(MoveList &move_list, U64 pawn_single_pushes, int pawn_single_push_offset)
  {
    U64 pawn_single_pushes_promo = pawn_single_pushes & (tables::MASK_RANK[0] | tables::MASK_RANK[7]);
    while (pawn_single_pushes_promo)
    {
      int to_square = utils::bitScanForward(pawn_single_pushes_promo);
      int from_square = to_square + pawn_single_push_offset;
      move_list.push_back(Move(from_square, to_square, PAWN, KNIGHT, false, false, false, false));
      move_list.push_back(Move(from_square, to_square, PAWN, BISHOP, false, false, false, false));
      move_list.push_back(Move(from_square, to_square, PAWN, ROOK, false, false, false, false));
      move_list.push_back(Move(from_square, to_square, PAWN, QUEEN, false, false, false, false));
      utils::popLastBit(pawn_single_pushes_promo);
    }
  }

  void generatePawnSinglePushNoPromotion(MoveList &move_list, U64 pawn_single_pushes, int pawn_single_push_offset)
  {
    U64 pawn_single_pushes_no_promo = pawn_single_pushes & tables::MASK_CLEAR_RANK[0] & tables::MASK_CLEAR_RANK[7];
    while (pawn_single_pushes_no_promo)
    {
      int to_square = utils::bitScanForward(pawn_single_pushes_no_promo);
      int from_square = to_square + pawn_single_push_offset;
      move_list.push_back(Move(from_square, to_square, PAWN, 0, false, false, false, false));
      utils::popLastBit(pawn_single_pushes_no_promo);
    }
  }

  void generatePawnCapturesWithPromotion(MoveList &move_list, int to_move, U64 to_move_pawns, U64 opponent_occupancies)
  {
    U64 pawns_can_capture_with_promo = to_move_pawns & tables::MASK_RANK[6 - (5 * to_move)];
    while (pawns_can_capture_with_promo)
    {
      int from_square = utils::bitScanForward(pawns_can_capture_with_promo);
      U64 pawn_captures_promo = tables::ATTACKS_PAWN[to_move][from_square] & opponent_occupancies;
      while (pawn_captures_promo)
      {
        int to_square = utils::bitScanForward(pawn_captures_promo);
        move_list.push_back(Move(from_square, to_square, PAWN, KNIGHT, true, false, false, false));
        move_list.push_back(Move(from_square, to_square, PAWN, BISHOP, true, false, false, false));
        move_list.push_back(Move(from_square, to_square, PAWN, ROOK, true, false, false, false));
        move_list.push_back(Move(from_square, to_square, PAWN, QUEEN, true, false, false, false));
        utils::popLastBit(pawn_captures_promo);
      }
      utils::popLastBit(pawns_can_capture_with_promo);
    }
  }

  void generatePawnCapturesNoPromotion(MoveList &move_list, int to_move, U64 to_move_pawns, U64 opponent_occupancies)
  {
    U64 pawns_can_capture_no_promo = to_move_pawns & tables::MASK_CLEAR_RANK[6 - (5 * to_move)];
    while (pawns_can_capture_no_promo)
    {
      int from_square = utils::bitScanForward(pawns_can_capture_no_promo);
      U64 pawn_captures_no_promo = tables::ATTACKS_PAWN[to_move][from_square] & opponent_occupancies;
      while (pawn_captures_no_promo)
      {
        int to_square = utils::bitScanForward(pawn_captures_no_promo);
        move_list.push_back(Move(from_square, to_square, PAWN, 0, true, false, false, false));
        utils::popLastBit(pawn_captures_no_promo);
      }
      utils::popLastBit(pawns_can_capture_no_promo);
    }
  }

  void generateEnPassantCapture(MoveList &move_list, U64 to_move_pawns, int en_passant_square, int opponent)
  {
    if (en_passant_square != -1)
    {
      U64 pawns_can_en_passant = tables::ATTACKS_PAWN[opponent][en_passant_square] & to_move_pawns;
      while (pawns_can_en_passant)
      {
        int from_square = utils::bitScanForward(pawns_can_en_passant);
        move_list.push_back(Move(from_square, en_passant_square, PAWN, 0, true, false, true, false));
        utils::popLastBit(pawns_can_en_passant);
      }
    }
  }

  void generateKnightMoves(MoveList &move_list, U64 to_move_knights, U64 to_move_occupancies, U64 opponent_occupancies)
  {
    while (to_move_knights)
    {
      int from_square = utils::bitScanForward(to_move_knights);
      U64 moves = tables::ATTACKS_KNIGHT[from_square] & ~to_move_occupancies;
      while (moves)
      {
        int to_square = utils::bitScanForward(moves);
        move_list.push_back(Move(from_square, to_square, KNIGHT, 0, utils::getBit(opponent_occupancies, to_square), false, false, false));
        utils::popLastBit(moves);
      }
      utils::popLastBit(to_move_knights);
    }
  }

  void generateBishopMoves(MoveList &move_list, U64 to_move_bishops, U64 to_move_occupancies, U64 opponent_occupancies, U64 both_occupancies)
  {
    while (to_move_bishops)
    {
      int from_square = utils::bitScanForward(to_move_bishops);
      U64 moves = tables::getBishopAttacks(from_square, both_occupancies) & ~to_move_occupancies;
      while (moves)
      {
        int to_square = utils::bitScanForward(moves);
        move_list.push_back(Move(from_square, to_square, BISHOP, 0, utils::getBit(opponent_occupancies, to_square), false, false, false));
        utils::popLastBit(moves);
      }
      utils::popLastBit(to_move_bishops);
    }
  }

  void generateRookMoves(MoveList &move_list, U64 to_move_rooks, U64 to_move_occupancies, U64 opponent_occupancies, U64 both_occupancies)
  {
    while (to_move_rooks)
    {
      int from_square = utils::bitScanForward(to_move_rooks);
      U64 moves = tables::getRookAttacks(from_square, both_occupancies) & ~to_move_occupancies;
      while (moves)
      {
        int to_square = utils::bitScanForward(moves);
        move_list.push_back(Move(from_square, to_square, ROOK, 0, utils::getBit(opponent_occupancies, to_square), false, false, false));
        utils::popLastBit(moves);
      }
      utils::popLastBit(to_move_rooks);
    }
  }

  void generateQueenMoves(MoveList &move_list, U64 to_move_queens, U64 to_move_occupancies, U64 opponent_occupancies, U64 both_occupancies)
  {
    while (to_move_queens)
    {
      int from_square = utils::bitScanForward(to_move_queens);
      U64 moves = tables::getQueenAttacks(from_square, both_occupancies) & ~to_move_occupancies;
      while (moves)
      {
        int to_square = utils::bitScanForward(moves);
        move_list.push_back(Move(from_square, to_square, QUEEN, 0, utils::getBit(opponent_occupancies, to_square), false, false, false));
        utils::popLastBit(moves);
      }
      utils::popLastBit(to_move_queens);
    }
  }

  void generateKingMoves(MoveList &move_list, U64 to_move_king, U64 to_move_occupancies, U64 opponent_occupancies)
  {
    int from_square = utils::bitScanForward(to_move_king);
    U64 moves = tables::ATTACKS_KING[from_square] & ~to_move_occupancies;
    while (moves)
    {
      int to_square = utils::bitScanForward(moves);
      move_list.push_back(Move(from_square, to_square, KING, 0, utils::getBit(opponent_occupancies, to_square), false, false, false));
      utils::popLastBit(moves);
    }
  }

}