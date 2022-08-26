#include <engine/movegen/movegen.hpp>

#include <engine/bitboard.hpp>
#include <engine/movegen/attacks.hpp>
#include <engine/movegen/tables.hpp>
#include <engine/board.hpp>
#include <engine/move.hpp>

#define MAX_SIZE_MOVES_ARRAY 256

namespace movegen
{
  enum GenType
  {
    QUIETS,
    CAPTURES
  };

  // Castling
  template <Color ToMove>
  static void generate_castling_moves(std::vector<Move> &move_list, const Board &board);

  // Pawns
  template <Color ToMove>
  static void generate_en_passant_capture(std::vector<Move> &move_list, const Board &board);
  template <Color ToMove>
  static void generate_pawn_single_push_with_promotion(std::vector<Move> &move_list, const Board &board);
  template <Color ToMove>
  static void generate_pawn_single_push_no_promotion(std::vector<Move> &move_list, const Board &board);
  template <Color ToMove>
  static void generate_pawn_captures_with_promotion(std::vector<Move> &move_list, const Board &board);
  template <Color ToMove>
  static void generate_pawn_captures_no_promotion(std::vector<Move> &move_list, const Board &board);
  template <Color ToMove>
  static void generate_pawn_double_pushes(std::vector<Move> &move_list, const Board &board);

  // Leaper Pieces
  template <Color ToMove, PieceType PType, GenType GType>
  static void generate_leaper_moves(std::vector<Move> &move_list, const Board &board);

  // Slider Pieces
  template <Color ToMove, PieceType PType, GenType GType>
  static void generate_slider_moves(std::vector<Move> &move_list, const Board &board);

  bool has_legal_moves(Board &board)
  {
    Board::GameState state = board.get_state();
    for (const Move &move : movegen::generate_pseudo_legal_moves(board))
    {
      board.make(move);
      Square king_sq = bitboard::bit_scan_forward(board.get_pieces(board.get_opponent(), KING));
      if (!board.is_square_attacked(king_sq, board.get_side_to_move()))
      {
        board.unmake(move, state);
        return true;
      }
      board.unmake(move, state);
    }
    return false;
  }

  // All Pseudo Legal Moves
  std::vector<Move> generate_pseudo_legal_moves(const Board &board)
  {
    std::vector<Move> moves;
    moves.reserve(MAX_SIZE_MOVES_ARRAY);

    if (board.get_side_to_move() == WHITE)
    {
      generate_pawn_captures_with_promotion<WHITE>(moves, board);
      generate_pawn_captures_no_promotion<WHITE>(moves, board);
      generate_en_passant_capture<WHITE>(moves, board);
      generate_pawn_single_push_with_promotion<WHITE>(moves, board);
      generate_pawn_double_pushes<WHITE>(moves, board);
      generate_pawn_single_push_no_promotion<WHITE>(moves, board);

      generate_leaper_moves<WHITE, KNIGHT, CAPTURES>(moves, board);
      generate_leaper_moves<WHITE, KING, CAPTURES>(moves, board);
      generate_slider_moves<WHITE, BISHOP, CAPTURES>(moves, board);
      generate_slider_moves<WHITE, ROOK, CAPTURES>(moves, board);
      generate_slider_moves<WHITE, QUEEN, CAPTURES>(moves, board);

      generate_leaper_moves<WHITE, KNIGHT, QUIETS>(moves, board);
      generate_leaper_moves<WHITE, KING, QUIETS>(moves, board);
      generate_slider_moves<WHITE, BISHOP, QUIETS>(moves, board);
      generate_slider_moves<WHITE, ROOK, QUIETS>(moves, board);
      generate_slider_moves<WHITE, QUEEN, QUIETS>(moves, board);

      generate_castling_moves<WHITE>(moves, board);
    }
    else
    {
      generate_pawn_captures_with_promotion<BLACK>(moves, board);
      generate_pawn_captures_no_promotion<BLACK>(moves, board);
      generate_en_passant_capture<BLACK>(moves, board);
      generate_pawn_single_push_with_promotion<BLACK>(moves, board);
      generate_pawn_double_pushes<BLACK>(moves, board);
      generate_pawn_single_push_no_promotion<BLACK>(moves, board);

      generate_leaper_moves<BLACK, KNIGHT, CAPTURES>(moves, board);
      generate_leaper_moves<BLACK, KING, CAPTURES>(moves, board);
      generate_slider_moves<BLACK, BISHOP, CAPTURES>(moves, board);
      generate_slider_moves<BLACK, ROOK, CAPTURES>(moves, board);
      generate_slider_moves<BLACK, QUEEN, CAPTURES>(moves, board);

      generate_leaper_moves<BLACK, KNIGHT, QUIETS>(moves, board);
      generate_leaper_moves<BLACK, KING, QUIETS>(moves, board);
      generate_slider_moves<BLACK, BISHOP, QUIETS>(moves, board);
      generate_slider_moves<BLACK, ROOK, QUIETS>(moves, board);
      generate_slider_moves<BLACK, QUEEN, QUIETS>(moves, board);

      generate_castling_moves<BLACK>(moves, board);
    }

    return moves;
  }

  std::vector<Move> generate_pseudo_legal_captures(const Board &board)
  {
    std::vector<Move> moves;
    moves.reserve(MAX_SIZE_MOVES_ARRAY);

    if (board.get_side_to_move() == WHITE)
    {
      generate_pawn_captures_with_promotion<WHITE>(moves, board);
      generate_pawn_captures_no_promotion<WHITE>(moves, board);
      generate_en_passant_capture<WHITE>(moves, board);

      generate_leaper_moves<WHITE, KNIGHT, CAPTURES>(moves, board);
      generate_leaper_moves<WHITE, KING, CAPTURES>(moves, board);

      generate_slider_moves<WHITE, BISHOP, CAPTURES>(moves, board);
      generate_slider_moves<WHITE, ROOK, CAPTURES>(moves, board);
      generate_slider_moves<WHITE, QUEEN, CAPTURES>(moves, board);
    }
    else
    {
      generate_pawn_captures_with_promotion<BLACK>(moves, board);
      generate_pawn_captures_no_promotion<BLACK>(moves, board);
      generate_en_passant_capture<BLACK>(moves, board);

      generate_leaper_moves<BLACK, KNIGHT, CAPTURES>(moves, board);
      generate_leaper_moves<BLACK, KING, CAPTURES>(moves, board);

      generate_slider_moves<BLACK, BISHOP, CAPTURES>(moves, board);
      generate_slider_moves<BLACK, ROOK, CAPTURES>(moves, board);
      generate_slider_moves<BLACK, QUEEN, CAPTURES>(moves, board);
    }

    return moves;
  }

  std::vector<Move> generateLegalMoves(Board &board)
  {
    std::vector<Move> moves;
    moves.reserve(MAX_SIZE_MOVES_ARRAY);

    Board::GameState state = board.get_state();
    for (const Move &move : movegen::generate_pseudo_legal_moves(board))
    {
      board.make(move);
      Color attacker_side = board.get_side_to_move();
      Square king_sq = bitboard::bit_scan_forward(board.get_pieces(board.get_opponent(), KING));
      if (!board.is_square_attacked(king_sq, attacker_side))
      {
        moves.push_back(move);
      }
      board.unmake(move, state);
    }
    return moves;
  }

  std::vector<Move> generate_legal_captures(Board &board)
  {
    std::vector<Move> captures;
    captures.reserve(MAX_SIZE_MOVES_ARRAY);

    Board::GameState state = board.get_state();
    for (const Move &move : movegen::generate_pseudo_legal_captures(board))
    {
      board.make(move);
      Color attacker_side = board.get_side_to_move();
      Square king_sq = bitboard::bit_scan_forward(board.get_pieces(board.get_opponent(), KING));
      if (!board.is_square_attacked(king_sq, attacker_side))
      {
        captures.push_back(move);
      }
      board.unmake(move, state);
    }
    return captures;
  }

  template <Color ToMove>
  static void generate_castling_moves(std::vector<Move> &move_list, const Board &board)
  {
    constexpr Color Opponent = (ToMove == WHITE ? BLACK : WHITE);
    constexpr Square CASTLE_B_SQ = (ToMove == WHITE ? B1 : B8);
    constexpr Square CASTLE_C_SQ = (ToMove == WHITE ? C1 : C8);
    constexpr Square CASTLE_D_SQ = (ToMove == WHITE ? D1 : D8);
    constexpr Square CASTLE_E_SQ = (ToMove == WHITE ? E1 : E8);
    constexpr Square CASTLE_F_SQ = (ToMove == WHITE ? F1 : F8);
    constexpr Square CASTLE_G_SQ = (ToMove == WHITE ? G1 : G8);
    constexpr CastlingRight CASTLE_KING_MASK = (ToMove == WHITE ? CASTLE_KING_WHITE : CASTLE_KING_BLACK);
    constexpr CastlingRight CASTLE_QUEEN_MASK = (ToMove == WHITE ? CASTLE_QUEEN_WHITE : CASTLE_QUEEN_BLACK);
    if (!board.is_square_attacked(CASTLE_E_SQ, Opponent))
    {
      if ((board.get_castling_rights() & CASTLE_KING_MASK) && !bitboard::get_bit(board.get_occupancies(BOTH), CASTLE_F_SQ) && !bitboard::get_bit(board.get_occupancies(BOTH), CASTLE_G_SQ))
      {
        if (!board.is_square_attacked(CASTLE_F_SQ, Opponent) && !board.is_square_attacked(CASTLE_G_SQ, Opponent))
        {
          move_list.emplace_back(CASTLE_E_SQ, CASTLE_G_SQ, KING, EMPTY_PIECE, EMPTY_PIECE, false, false, true);
        }
      }
      if ((board.get_castling_rights() & CASTLE_QUEEN_MASK) && !bitboard::get_bit(board.get_occupancies(BOTH), CASTLE_D_SQ) && !bitboard::get_bit(board.get_occupancies(BOTH), CASTLE_C_SQ) && !bitboard::get_bit(board.get_occupancies(BOTH), CASTLE_B_SQ))
      {
        if (!board.is_square_attacked(CASTLE_D_SQ, Opponent) && !board.is_square_attacked(CASTLE_C_SQ, Opponent))
        {
          move_list.emplace_back(CASTLE_E_SQ, CASTLE_C_SQ, KING, EMPTY_PIECE, EMPTY_PIECE, false, false, true);
        }
      }
    }
  }

  template <Color ToMove>
  static void generate_pawn_double_pushes(std::vector<Move> &move_list, const Board &board)
  {
    constexpr int PAWN_DOUBLE_PUSH_OFFSET = ToMove == WHITE ? -16 : 16;
    constexpr u64 (*MASK_FUNC)(u64 wpawns, u64 empty) = ToMove == WHITE ? attacks::mask_white_pawn_double_pushes : attacks::mask_black_pawn_double_pushes;
    u64 pawn_double_pushes = MASK_FUNC(board.get_pieces(ToMove, PAWN), ~board.get_occupancies(BOTH));
    while (pawn_double_pushes)
    {
      int to_square = bitboard::bit_scan_forward(pawn_double_pushes);
      int from_square = to_square + PAWN_DOUBLE_PUSH_OFFSET;
      move_list.emplace_back(from_square, to_square, PAWN, EMPTY_PIECE, EMPTY_PIECE, true, false, false);
      bitboard::pop_last_bit(pawn_double_pushes);
    }
  }

  template <Color ToMove>
  static void generate_pawn_single_push_with_promotion(std::vector<Move> &move_list, const Board &board)
  {
    constexpr int PAWN_SINGLE_PUSH_OFFSET = ToMove == WHITE ? -8 : 8;
    constexpr u64 (*MASK_FUNC)(u64 wpawns, u64 empty) = ToMove == WHITE ? attacks::mask_white_pawn_single_pushes : attacks::mask_black_pawn_single_pushes;
    u64 pawn_single_pushes = MASK_FUNC(board.get_pieces(ToMove, PAWN), ~board.get_occupancies(BOTH));
    u64 pawn_single_pushes_promo = pawn_single_pushes & (tables::MASK_RANK[0] | tables::MASK_RANK[7]);
    while (pawn_single_pushes_promo)
    {
      int to_square = bitboard::bit_scan_forward(pawn_single_pushes_promo);
      int from_square = to_square + PAWN_SINGLE_PUSH_OFFSET;
      move_list.emplace_back(from_square, to_square, PAWN, EMPTY_PIECE, QUEEN, false, false, false);
      move_list.emplace_back(from_square, to_square, PAWN, EMPTY_PIECE, KNIGHT, false, false, false);
      move_list.emplace_back(from_square, to_square, PAWN, EMPTY_PIECE, ROOK, false, false, false);
      move_list.emplace_back(from_square, to_square, PAWN, EMPTY_PIECE, BISHOP, false, false, false);
      bitboard::pop_last_bit(pawn_single_pushes_promo);
    }
  }

  template <Color ToMove>
  static void generate_pawn_single_push_no_promotion(std::vector<Move> &move_list, const Board &board)
  {
    constexpr int PAWN_SINGLE_PUSH_OFFSET = ToMove == WHITE ? -8 : 8;
    constexpr u64 (*MASK_FUNC)(u64 wpawns, u64 empty) = ToMove == WHITE ? attacks::mask_white_pawn_single_pushes : attacks::mask_black_pawn_single_pushes;
    u64 pawn_single_pushes = MASK_FUNC(board.get_pieces(ToMove, PAWN), ~board.get_occupancies(BOTH));
    u64 pawn_single_pushes_no_promo = pawn_single_pushes & tables::MASK_CLEAR_RANK[0] & tables::MASK_CLEAR_RANK[7];
    while (pawn_single_pushes_no_promo)
    {
      int to_square = bitboard::bit_scan_forward(pawn_single_pushes_no_promo);
      int from_square = to_square + PAWN_SINGLE_PUSH_OFFSET;
      move_list.emplace_back(from_square, to_square, PAWN, EMPTY_PIECE, EMPTY_PIECE, false, false, false);
      bitboard::pop_last_bit(pawn_single_pushes_no_promo);
    }
  }

  template <Color ToMove>
  static void generate_pawn_captures_with_promotion(std::vector<Move> &move_list, const Board &board)
  {
    constexpr Color Opponent = ToMove == WHITE ? BLACK : WHITE;
    constexpr int MASK_INDEX = ToMove == WHITE ? 6 : 1;
    u64 pawns_can_capture_with_promo = board.get_pieces(ToMove, PAWN) & tables::MASK_RANK[MASK_INDEX];
    while (pawns_can_capture_with_promo)
    {
      Square from_square = bitboard::bit_scan_forward(pawns_can_capture_with_promo);
      u64 pawn_captures_promo = tables::get_pawn_attacks(ToMove, from_square) & board.get_occupancies(Opponent);
      while (pawn_captures_promo)
      {
        Square to_square = bitboard::bit_scan_forward(pawn_captures_promo);
        move_list.emplace_back(from_square, to_square, PAWN, board.get_piece_from_square(to_square).type, QUEEN, false, false, false);
        move_list.emplace_back(from_square, to_square, PAWN, board.get_piece_from_square(to_square).type, KNIGHT, false, false, false);
        move_list.emplace_back(from_square, to_square, PAWN, board.get_piece_from_square(to_square).type, ROOK, false, false, false);
        move_list.emplace_back(from_square, to_square, PAWN, board.get_piece_from_square(to_square).type, BISHOP, false, false, false);
        bitboard::pop_last_bit(pawn_captures_promo);
      }
      bitboard::pop_last_bit(pawns_can_capture_with_promo);
    }
  }

  template <Color ToMove>
  static void generate_pawn_captures_no_promotion(std::vector<Move> &move_list, const Board &board)
  {
    constexpr Color Opponent = ToMove == WHITE ? BLACK : WHITE;
    constexpr int MASK_INDEX = ToMove == WHITE ? 6 : 1;
    u64 pawns_can_capture_no_promo = board.get_pieces(ToMove, PAWN) & tables::MASK_CLEAR_RANK[MASK_INDEX];
    while (pawns_can_capture_no_promo)
    {
      Square from_square = bitboard::bit_scan_forward(pawns_can_capture_no_promo);
      u64 pawn_captures_no_promo = tables::get_pawn_attacks(ToMove, from_square) & board.get_occupancies(Opponent);
      while (pawn_captures_no_promo)
      {
        Square to_square = bitboard::bit_scan_forward(pawn_captures_no_promo);
        move_list.emplace_back(from_square, to_square, PAWN, board.get_piece_from_square(to_square).type, EMPTY_PIECE, false, false, false);
        bitboard::pop_last_bit(pawn_captures_no_promo);
      }
      bitboard::pop_last_bit(pawns_can_capture_no_promo);
    }
  }

  template <Color ToMove>
  static void generate_en_passant_capture(std::vector<Move> &move_list, const Board &board)
  {
    constexpr Color Opponent = ToMove == WHITE ? BLACK : WHITE;
    if (board.get_en_passant_square() != -1)
    {
      u64 pawns_can_en_passant = tables::get_pawn_attacks(Opponent, board.get_en_passant_square()) & board.get_pieces(ToMove, PAWN);
      while (pawns_can_en_passant)
      {
        int from_square = bitboard::bit_scan_forward(pawns_can_en_passant);
        move_list.emplace_back(from_square, board.get_en_passant_square(), PAWN, board.get_piece_from_square(board.get_en_passant_square()).type, EMPTY_PIECE, false, true, false);
        bitboard::pop_last_bit(pawns_can_en_passant);
      }
    }
  }

  template <Color ToMove, PieceType PType, GenType GType>
  static void generate_leaper_moves(std::vector<Move> &move_list, const Board &board)
  {
    static_assert(PType == KNIGHT || PType == KING, "Unsupported piece type in generateLeaperMoves()");

    constexpr Color Opponent = ToMove == WHITE ? BLACK : WHITE;
    constexpr u64 (*ATTACKS_FUNC)(Square sq) = PType == KNIGHT ? tables::get_knight_attacks : tables::get_king_attacks;
    u64 to_move_occupancies = board.get_occupancies(ToMove);
    u64 opponent_occupancies = board.get_occupancies(Opponent);
    u64 to_move_pieces = board.get_pieces(ToMove, PType);
    while (to_move_pieces)
    {
      Square from_square = bitboard::bit_scan_forward(to_move_pieces);
      u64 moves;
      if constexpr (GType == QUIETS)
      {
        moves = ATTACKS_FUNC(from_square) & ~to_move_occupancies & ~opponent_occupancies;
      }
      else
      {
        moves = ATTACKS_FUNC(from_square) & ~to_move_occupancies & opponent_occupancies;
      }
      while (moves)
      {
        Square to_square = bitboard::bit_scan_forward(moves);
        if constexpr (GType == QUIETS)
        {
          move_list.emplace_back(from_square, to_square, PType, EMPTY_PIECE, EMPTY_PIECE, false, false, false);
        }
        else
        {
          move_list.emplace_back(from_square, to_square, PType, board.get_piece_from_square(to_square).type, EMPTY_PIECE, false, false, false);
        }
        bitboard::pop_last_bit(moves);
      }
      bitboard::pop_last_bit(to_move_pieces);
    }
  }

  template <Color ToMove, PieceType PType, GenType GType>
  static void generate_slider_moves(std::vector<Move> &move_list, const Board &board)
  {
    static_assert(PType == BISHOP || PType == ROOK || PType == QUEEN, "Unsupported piece type in generate_slider_moves()");

    constexpr Color Opponent = ToMove == WHITE ? BLACK : WHITE;
    constexpr u64 (*ATTACKS_FUNC)(const Square sq, u64 occ) = (PType == BISHOP ? tables::get_bishop_attacks
                                                               : PType == ROOK ? tables::get_rook_attacks
                                                                               : tables::get_queen_attacks);
    u64 to_move_occupancies = board.get_occupancies(ToMove);
    u64 opponent_occupancies = board.get_occupancies(Opponent);
    u64 to_move_pieces = board.get_pieces(ToMove, PType);
    while (to_move_pieces)
    {
      Square from_square = bitboard::bit_scan_forward(to_move_pieces);
      u64 moves;
      if constexpr (GType == QUIETS)
      {
        moves = ATTACKS_FUNC(from_square, board.get_occupancies(BOTH)) & ~to_move_occupancies & ~opponent_occupancies;
      }
      else
      {
        moves = ATTACKS_FUNC(from_square, board.get_occupancies(BOTH)) & ~to_move_occupancies & opponent_occupancies;
      }
      while (moves)
      {
        Square to_square = bitboard::bit_scan_forward(moves);
        if constexpr (GType == QUIETS)
        {
          move_list.emplace_back(from_square, to_square, PType, EMPTY_PIECE, EMPTY_PIECE, false, false, false);
        }
        else
        {
          move_list.emplace_back(from_square, to_square, PType, board.get_piece_from_square(to_square).type, EMPTY_PIECE, false, false, false);
        }
        bitboard::pop_last_bit(moves);
      }
      bitboard::pop_last_bit(to_move_pieces);
    }
  }

} // namespace movegen