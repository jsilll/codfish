#include "movegen.hpp"

#include <engine/movegen/bitboard.hpp>
#include <engine/movegen/attacks.hpp>
#include <engine/movegen/tables.hpp>
#include <engine/movegen/board.hpp>
#include <engine/movegen/move.hpp>

#define MAX_SIZE_MOVES_ARRAY 256

namespace movegen
{
  enum GenType
  {
    QUIETS,
    CAPTURES
  };

  // Castling
  template <PieceColor ToMove>
  void generateCastlingMoves(std::vector<Move> &move_list, const Board &board);

  // Pawns
  template <PieceColor ToMove>
  void generateEnPassantCapture(std::vector<Move> &move_list, const Board &board);
  template <PieceColor ToMove>
  void generatePawnSinglePushWithPromotion(std::vector<Move> &move_list, const Board &board);
  template <PieceColor ToMove>
  void generatePawnSinglePushNoPromotion(std::vector<Move> &move_list, const Board &board);
  template <PieceColor ToMove>
  void generatePawnCapturesWithPromotion(std::vector<Move> &move_list, const Board &board);
  template <PieceColor ToMove>
  void generatePawnCapturesNoPromotion(std::vector<Move> &move_list, const Board &board);
  template <PieceColor ToMove>
  void generatePawnDoublePushes(std::vector<Move> &move_list, const Board &board);

  // Leaper Pieces
  template <PieceColor ToMove, PieceType PType, GenType GType>
  void generateLeaperMoves(std::vector<Move> &move_list, const Board &board);

  // Slider Pieces
  template <PieceColor ToMove, PieceType PType, GenType GType>
  void generateSliderMoves(std::vector<Move> &move_list, const Board &board);

  bool hasLegalMoves(const Board &board) // TODO: performance can be improved
  {
    for (const Move &move : movegen::generatePseudoLegalMoves(board))
    {
      Board backup = board;
      backup.makeMove(move);
      int king_sq = bitboard::bitScanForward(backup.getPieces(backup.getOpponent(), KING));
      if (!backup.isSquareAttacked(king_sq, backup.getSideToMove()))
      {
        return true;
      }
    }
    return false;
  }

  // All Pseudo Legal Moves
  std::vector<Move> generatePseudoLegalMoves(const Board &board)
  {
    std::vector<Move> moves;
    moves.reserve(MAX_SIZE_MOVES_ARRAY);
    if (board.getSideToMove() == WHITE)
    {
      generatePawnCapturesWithPromotion<WHITE>(moves, board);
      generatePawnCapturesNoPromotion<WHITE>(moves, board);
      generateEnPassantCapture<WHITE>(moves, board);
      generatePawnSinglePushWithPromotion<WHITE>(moves, board);
      generatePawnDoublePushes<WHITE>(moves, board);
      generatePawnSinglePushNoPromotion<WHITE>(moves, board);

      generateLeaperMoves<WHITE, KNIGHT, CAPTURES>(moves, board);
      generateLeaperMoves<WHITE, KING, CAPTURES>(moves, board);
      generateSliderMoves<WHITE, BISHOP, CAPTURES>(moves, board);
      generateSliderMoves<WHITE, ROOK, CAPTURES>(moves, board);
      generateSliderMoves<WHITE, QUEEN, CAPTURES>(moves, board);

      generateLeaperMoves<WHITE, KNIGHT, QUIETS>(moves, board);
      generateLeaperMoves<WHITE, KING, QUIETS>(moves, board);
      generateSliderMoves<WHITE, BISHOP, QUIETS>(moves, board);
      generateSliderMoves<WHITE, ROOK, QUIETS>(moves, board);
      generateSliderMoves<WHITE, QUEEN, QUIETS>(moves, board);

      generateCastlingMoves<WHITE>(moves, board);
    }
    else
    {
      generatePawnCapturesWithPromotion<BLACK>(moves, board);
      generatePawnCapturesNoPromotion<BLACK>(moves, board);
      generateEnPassantCapture<BLACK>(moves, board);
      generatePawnSinglePushWithPromotion<BLACK>(moves, board);
      generatePawnDoublePushes<BLACK>(moves, board);
      generatePawnSinglePushNoPromotion<BLACK>(moves, board);

      generateLeaperMoves<BLACK, KNIGHT, CAPTURES>(moves, board);
      generateLeaperMoves<BLACK, KING, CAPTURES>(moves, board);
      generateSliderMoves<BLACK, BISHOP, CAPTURES>(moves, board);
      generateSliderMoves<BLACK, ROOK, CAPTURES>(moves, board);
      generateSliderMoves<BLACK, QUEEN, CAPTURES>(moves, board);

      generateLeaperMoves<BLACK, KNIGHT, QUIETS>(moves, board);
      generateLeaperMoves<BLACK, KING, QUIETS>(moves, board);
      generateSliderMoves<BLACK, BISHOP, QUIETS>(moves, board);
      generateSliderMoves<BLACK, ROOK, QUIETS>(moves, board);
      generateSliderMoves<BLACK, QUEEN, QUIETS>(moves, board);

      generateCastlingMoves<BLACK>(moves, board);
    }

    return moves;
  }

  std::vector<Move> generatePseudoLegalCaptures(const Board &board)
  {
    std::vector<Move> moves;
    moves.reserve(MAX_SIZE_MOVES_ARRAY);
    if (board.getSideToMove() == WHITE)
    {
      generatePawnCapturesWithPromotion<WHITE>(moves, board);
      generatePawnCapturesNoPromotion<WHITE>(moves, board);
      generateEnPassantCapture<WHITE>(moves, board);

      generateLeaperMoves<WHITE, KNIGHT, CAPTURES>(moves, board);
      generateLeaperMoves<WHITE, KING, CAPTURES>(moves, board);

      generateSliderMoves<WHITE, BISHOP, CAPTURES>(moves, board);
      generateSliderMoves<WHITE, ROOK, CAPTURES>(moves, board);
      generateSliderMoves<WHITE, QUEEN, CAPTURES>(moves, board);
    }
    else
    {
      generatePawnCapturesWithPromotion<BLACK>(moves, board);
      generatePawnCapturesNoPromotion<BLACK>(moves, board);
      generateEnPassantCapture<BLACK>(moves, board);

      generateLeaperMoves<BLACK, KNIGHT, CAPTURES>(moves, board);
      generateLeaperMoves<BLACK, KING, CAPTURES>(moves, board);

      generateSliderMoves<BLACK, BISHOP, CAPTURES>(moves, board);
      generateSliderMoves<BLACK, ROOK, CAPTURES>(moves, board);
      generateSliderMoves<BLACK, QUEEN, CAPTURES>(moves, board);
    }

    return moves;
  }

  std::vector<Move> generateLegalMoves(const Board &board)
  {
    std::vector<Move> moves;
    moves.reserve(MAX_SIZE_MOVES_ARRAY);
    for (Move const &move : movegen::generatePseudoLegalMoves(board))
    {
      Board backup = board;
      backup.makeMove(move);
      int king_sq = bitboard::bitScanForward(backup.getPieces(board.getSideToMove(), KING));
      int attacker_side = backup.getSideToMove();
      if (!backup.isSquareAttacked(king_sq, attacker_side))
      {
        moves.push_back(move);
      }
    }
    return moves;
  }

  std::vector<Move> generateLegalCaptures(const Board &board)
  {
    std::vector<Move> captures;
    captures.reserve(MAX_SIZE_MOVES_ARRAY);
    for (Move const &move : movegen::generatePseudoLegalCaptures(board))
    {
      Board backup = board;
      backup.makeMove(move);
      int king_sq = bitboard::bitScanForward(backup.getPieces(board.getSideToMove(), KING));
      int attacker_side = backup.getSideToMove();
      if (!backup.isSquareAttacked(king_sq, attacker_side))
      {
        captures.push_back(move);
      }
    }
    return captures;
  }

  template <PieceColor ToMove>
  void generateCastlingMoves(std::vector<Move> &move_list, const Board &board)
  {
    constexpr PieceColor Opponent = (ToMove == WHITE ? BLACK : WHITE);
    constexpr int Castle_B_Sq = (ToMove == WHITE ? B1 : B8);
    constexpr int Castle_C_Sq = (ToMove == WHITE ? C1 : C8);
    constexpr int Castle_D_Sq = (ToMove == WHITE ? D1 : D8);
    constexpr int Castle_E_Sq = (ToMove == WHITE ? E1 : E8);
    constexpr int Castle_F_Sq = (ToMove == WHITE ? F1 : F8);
    constexpr int Castle_G_Sq = (ToMove == WHITE ? G1 : G8);
    constexpr int Castle_King_Mask = (ToMove == WHITE ? CASTLE_KING_WHITE : CASTLE_KING_BLACK);
    constexpr int Castle_Queen_Mask = (ToMove == WHITE ? CASTLE_QUEEN_WHITE : CASTLE_QUEEN_BLACK);
    if (!board.isSquareAttacked(Castle_E_Sq, Opponent))
    {
      if ((board.getCastlingRights() & Castle_King_Mask) && !bitboard::getBit(board.getOccupancies(BOTH), Castle_F_Sq) && !bitboard::getBit(board.getOccupancies(BOTH), Castle_G_Sq))
      {
        if (!board.isSquareAttacked(Castle_F_Sq, Opponent) && !board.isSquareAttacked(Castle_G_Sq, Opponent))
        {
          move_list.emplace_back(Castle_E_Sq, Castle_G_Sq, KING, EMPTY_PIECE, EMPTY_PIECE, false, false, true);
        }
      }
      if ((board.getCastlingRights() & Castle_Queen_Mask) && !bitboard::getBit(board.getOccupancies(BOTH), Castle_D_Sq) && !bitboard::getBit(board.getOccupancies(BOTH), Castle_C_Sq) && !bitboard::getBit(board.getOccupancies(BOTH), Castle_B_Sq))
      {
        if (!board.isSquareAttacked(Castle_D_Sq, Opponent) && !board.isSquareAttacked(Castle_C_Sq, Opponent))
        {
          move_list.emplace_back(Castle_E_Sq, Castle_C_Sq, KING, EMPTY_PIECE, EMPTY_PIECE, false, false, true);
        }
      }
    }
  }

  template <PieceColor ToMove>
  void generatePawnDoublePushes(std::vector<Move> &move_list, const Board &board)
  {
    constexpr int Pawn_Double_Push_Offset = ToMove == WHITE ? -16 : 16;
    constexpr U64 (*mask_func)(U64 wpawns, U64 empty) = ToMove == WHITE ? attacks::maskWhitePawnDoublePushes : attacks::maskBlackPawnDoublePushes;
    U64 pawn_double_pushes = mask_func(board.getPieces(ToMove, PAWN), ~board.getOccupancies(BOTH));
    while (pawn_double_pushes)
    {
      int to_square = bitboard::bitScanForward(pawn_double_pushes);
      int from_square = to_square + Pawn_Double_Push_Offset;
      move_list.emplace_back(from_square, to_square, PAWN, EMPTY_PIECE, EMPTY_PIECE, true, false, false);
      bitboard::popLastBit(pawn_double_pushes);
    }
  }

  template <PieceColor ToMove>
  void generatePawnSinglePushWithPromotion(std::vector<Move> &move_list, const Board &board)
  {
    constexpr int Pawn_Single_Push_Offset = ToMove == WHITE ? -8 : 8;
    constexpr U64 (*mask_func)(U64 wpawns, U64 empty) = ToMove == WHITE ? attacks::maskWhitePawnSinglePushes : attacks::maskBlackPawnSinglePushes;
    U64 pawn_single_pushes = mask_func(board.getPieces(ToMove, PAWN), ~board.getOccupancies(BOTH));
    U64 pawn_single_pushes_promo = pawn_single_pushes & (tables::MASK_RANK[0] | tables::MASK_RANK[7]);
    while (pawn_single_pushes_promo)
    {
      int to_square = bitboard::bitScanForward(pawn_single_pushes_promo);
      int from_square = to_square + Pawn_Single_Push_Offset;
      move_list.emplace_back(from_square, to_square, PAWN, EMPTY_PIECE, QUEEN, false, false, false);
      move_list.emplace_back(from_square, to_square, PAWN, EMPTY_PIECE, KNIGHT, false, false, false);
      move_list.emplace_back(from_square, to_square, PAWN, EMPTY_PIECE, ROOK, false, false, false);
      move_list.emplace_back(from_square, to_square, PAWN, EMPTY_PIECE, BISHOP, false, false, false);
      bitboard::popLastBit(pawn_single_pushes_promo);
    }
  }

  template <PieceColor ToMove>
  void generatePawnSinglePushNoPromotion(std::vector<Move> &move_list, const Board &board)
  {
    constexpr int Pawn_Single_Push_Offset = ToMove == WHITE ? -8 : 8;
    constexpr U64 (*mask_func)(U64 wpawns, U64 empty) = ToMove == WHITE ? attacks::maskWhitePawnSinglePushes : attacks::maskBlackPawnSinglePushes;
    U64 pawn_single_pushes = mask_func(board.getPieces(ToMove, PAWN), ~board.getOccupancies(BOTH));
    U64 pawn_single_pushes_no_promo = pawn_single_pushes & tables::MASK_CLEAR_RANK[0] & tables::MASK_CLEAR_RANK[7];
    while (pawn_single_pushes_no_promo)
    {
      int to_square = bitboard::bitScanForward(pawn_single_pushes_no_promo);
      int from_square = to_square + Pawn_Single_Push_Offset;
      move_list.emplace_back(from_square, to_square, PAWN, EMPTY_PIECE, EMPTY_PIECE, false, false, false);
      bitboard::popLastBit(pawn_single_pushes_no_promo);
    }
  }

  template <PieceColor ToMove>
  void generatePawnCapturesWithPromotion(std::vector<Move> &move_list, const Board &board)
  {
    constexpr PieceColor Opponent = ToMove == WHITE ? BLACK : WHITE;
    constexpr int mask_index = 6 - (5 * ToMove);
    U64 pawns_can_capture_with_promo = board.getPieces(ToMove, PAWN) & tables::MASK_RANK[mask_index];
    while (pawns_can_capture_with_promo)
    {
      int from_square = bitboard::bitScanForward(pawns_can_capture_with_promo);
      U64 pawn_captures_promo = tables::ATTACKS_PAWN[ToMove][from_square] & board.getOccupancies(Opponent);
      while (pawn_captures_promo)
      {
        int to_square = bitboard::bitScanForward(pawn_captures_promo);
        move_list.emplace_back(from_square, to_square, PAWN, board.getPieceFromSquare(to_square).type, QUEEN, false, false, false);
        move_list.emplace_back(from_square, to_square, PAWN, board.getPieceFromSquare(to_square).type, KNIGHT, false, false, false);
        move_list.emplace_back(from_square, to_square, PAWN, board.getPieceFromSquare(to_square).type, ROOK, false, false, false);
        move_list.emplace_back(from_square, to_square, PAWN, board.getPieceFromSquare(to_square).type, BISHOP, false, false, false);
        bitboard::popLastBit(pawn_captures_promo);
      }
      bitboard::popLastBit(pawns_can_capture_with_promo);
    }
  }

  template <PieceColor ToMove>
  void generatePawnCapturesNoPromotion(std::vector<Move> &move_list, const Board &board)
  {
    constexpr PieceColor Opponent = ToMove == WHITE ? BLACK : WHITE;
    constexpr int mask_index = 6 - (5 * ToMove);
    U64 pawns_can_capture_no_promo = board.getPieces(ToMove, PAWN) & tables::MASK_CLEAR_RANK[mask_index];
    while (pawns_can_capture_no_promo)
    {
      int from_square = bitboard::bitScanForward(pawns_can_capture_no_promo);
      U64 pawn_captures_no_promo = tables::ATTACKS_PAWN[ToMove][from_square] & board.getOccupancies(Opponent);
      while (pawn_captures_no_promo)
      {
        int to_square = bitboard::bitScanForward(pawn_captures_no_promo);
        move_list.emplace_back(from_square, to_square, PAWN, board.getPieceFromSquare(to_square).type, EMPTY_PIECE, false, false, false);
        bitboard::popLastBit(pawn_captures_no_promo);
      }
      bitboard::popLastBit(pawns_can_capture_no_promo);
    }
  }

  template <PieceColor ToMove>
  void generateEnPassantCapture(std::vector<Move> &move_list, const Board &board)
  {
    constexpr PieceColor Opponent = ToMove == WHITE ? BLACK : WHITE;
    if (board.getEnPassantSquare() != -1)
    {
      U64 pawns_can_en_passant = tables::ATTACKS_PAWN[Opponent][board.getEnPassantSquare()] & board.getPieces(ToMove, PAWN);
      while (pawns_can_en_passant)
      {
        int from_square = bitboard::bitScanForward(pawns_can_en_passant);
        move_list.emplace_back(from_square, board.getEnPassantSquare(), PAWN, board.getPieceFromSquare(board.getEnPassantSquare()).type, EMPTY_PIECE, false, true, false);
        bitboard::popLastBit(pawns_can_en_passant);
      }
    }
  }

  template <PieceColor ToMove, PieceType PType, GenType GType>
  void generateLeaperMoves(std::vector<Move> &move_list, const Board &board)
  {
    static_assert(PType == KNIGHT || PType == KING, "Unsupported piece type in generateLeaperMoves()");

    constexpr PieceColor Opponent = ToMove == WHITE ? BLACK : WHITE;
    constexpr U64 *attacks_table = PType == KNIGHT ? tables::ATTACKS_KNIGHT : tables::ATTACKS_KING;
    U64 to_move_occupancies = board.getOccupancies(ToMove);
    U64 opponent_occupancies = board.getOccupancies(Opponent);
    U64 to_move_pieces = board.getPieces(ToMove, PType);
    while (to_move_pieces)
    {
      int from_square = bitboard::bitScanForward(to_move_pieces);
      U64 moves;
      if constexpr (GType == QUIETS)
      {
        moves = attacks_table[from_square] & ~to_move_occupancies & ~opponent_occupancies;
      }
      else
      {
        moves = attacks_table[from_square] & ~to_move_occupancies & opponent_occupancies;
      }
      while (moves)
      {
        int to_square = bitboard::bitScanForward(moves);
        if constexpr (GType == QUIETS)
        {
          move_list.emplace_back(from_square, to_square, PType, EMPTY_PIECE, EMPTY_PIECE, false, false, false);
        }
        else
        {
          move_list.emplace_back(from_square, to_square, PType, board.getPieceFromSquare(to_square).type, EMPTY_PIECE, false, false, false);
        }
        bitboard::popLastBit(moves);
      }
      bitboard::popLastBit(to_move_pieces);
    }
  }

  template <PieceColor ToMove, PieceType PType, GenType GType>
  void generateSliderMoves(std::vector<Move> &move_list, const Board &board)
  {
    static_assert(PType == BISHOP || PType == ROOK || PType == QUEEN, "Unsupported piece type in generateSliderMoves()");

    constexpr PieceColor Opponent = ToMove == WHITE ? BLACK : WHITE;
    constexpr U64 (*attacks_getter)(int sq, U64 occ) = (PType == BISHOP ? tables::getBishopAttacks
                                                        : PType == ROOK ? tables::getRookAttacks
                                                                        : tables::getQueenAttacks);
    U64 to_move_occupancies = board.getOccupancies(ToMove);
    U64 opponent_occupancies = board.getOccupancies(Opponent);
    U64 to_move_pieces = board.getPieces(ToMove, PType);
    while (to_move_pieces)
    {
      int from_square = bitboard::bitScanForward(to_move_pieces);
      U64 moves;
      if constexpr (GType == QUIETS)
      {
        moves = attacks_getter(from_square, board.getOccupancies(BOTH)) & ~to_move_occupancies & ~opponent_occupancies;
      }
      else
      {
        moves = attacks_getter(from_square, board.getOccupancies(BOTH)) & ~to_move_occupancies & opponent_occupancies;
      }
      while (moves)
      {
        int to_square = bitboard::bitScanForward(moves);
        if constexpr (GType == QUIETS)
        {
          move_list.emplace_back(from_square, to_square, PType, EMPTY_PIECE, EMPTY_PIECE, false, false, false);
        }
        else
        {
          move_list.emplace_back(from_square, to_square, PType, board.getPieceFromSquare(to_square).type, EMPTY_PIECE, false, false, false);
        }
        bitboard::popLastBit(moves);
      }
      bitboard::popLastBit(to_move_pieces);
    }
  }

} // namespace movegen