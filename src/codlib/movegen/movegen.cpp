#include "codlib/movegen.hpp"

#include "attacks.hpp"
#include "codlib/bitboard.hpp"
#include <codlib/board.hpp>
#include <codlib/move.hpp>

#define MAX_SIZE_MOVES_ARRAY 256

using bitboard::u64;

namespace movegen {
    /// @brief The type of move generation to perform.
    enum GenType {
        /// @brief Generate capture moves.
        CAPTURES,
        /// @brief Generate quiet moves (non-captures).
        QUIETS,
    };

    /// @brief Generates all the castling moves for the given board.
    /// @tparam ToMove The color to generate castling moves for.
    /// @param move_list The list of moves to add the castling moves to.
    /// @param board The board to generate castling moves on.
    template<Color ToMove>
    static void generate_castling_moves(std::vector<Move> &move_list, const Board &board) noexcept {
        constexpr Color Opponent = (ToMove == WHITE ? BLACK : WHITE);
        constexpr Square CASTLE_B_SQ = (ToMove == WHITE ? B1 : B8);
        constexpr Square CASTLE_C_SQ = (ToMove == WHITE ? C1 : C8);
        constexpr Square CASTLE_D_SQ = (ToMove == WHITE ? D1 : D8);
        constexpr Square CASTLE_E_SQ = (ToMove == WHITE ? E1 : E8);
        constexpr Square CASTLE_F_SQ = (ToMove == WHITE ? F1 : F8);
        constexpr Square CASTLE_G_SQ = (ToMove == WHITE ? G1 : G8);
        constexpr CastlingRight CASTLE_KING_MASK = (ToMove == WHITE ? CASTLE_KING_WHITE : CASTLE_KING_BLACK);
        constexpr CastlingRight CASTLE_QUEEN_MASK = (ToMove == WHITE ? CASTLE_QUEEN_WHITE : CASTLE_QUEEN_BLACK);
        if (!board.is_square_attacked(CASTLE_E_SQ, Opponent)) {
            if ((board.get_castling_rights() & CASTLE_KING_MASK) && !bitboard::get_bit(board.get_occupancies(BOTH), CASTLE_F_SQ) && !bitboard::get_bit(board.get_occupancies(BOTH), CASTLE_G_SQ)) {
                if (!board.is_square_attacked(CASTLE_F_SQ, Opponent) && !board.is_square_attacked(CASTLE_G_SQ, Opponent)) {
                    move_list.emplace_back(CASTLE_E_SQ, CASTLE_G_SQ, KING, EMPTY_PIECE, EMPTY_PIECE, false, false, true);
                }
            }
            if ((board.get_castling_rights() & CASTLE_QUEEN_MASK) && !bitboard::get_bit(board.get_occupancies(BOTH), CASTLE_D_SQ) && !bitboard::get_bit(board.get_occupancies(BOTH), CASTLE_C_SQ) && !bitboard::get_bit(board.get_occupancies(BOTH), CASTLE_B_SQ)) {
                if (!board.is_square_attacked(CASTLE_D_SQ, Opponent) && !board.is_square_attacked(CASTLE_C_SQ, Opponent)) {
                    move_list.emplace_back(CASTLE_E_SQ, CASTLE_C_SQ, KING, EMPTY_PIECE, EMPTY_PIECE, false, false, true);
                }
            }
        }
    }

    /// @brief Generates all the en passant capture moves for the given board.
    /// @tparam ToMove The color to generate en passant capture moves for.
    /// @param move_list The list of moves to add the en passant capture moves to.
    /// @param board The board to generate en passant capture moves on.
    template<Color ToMove>
    static void generate_en_passant_capture(std::vector<Move> &move_list, const Board &board) noexcept {
        constexpr Color Opponent = ToMove == WHITE ? BLACK : WHITE;
        if (board.get_en_passant_square() != -1) {
            u64 pawns_can_en_passant = attacks::ATTACKS_PAWN[Opponent][board.get_en_passant_square()] & board.get_pieces(ToMove, PAWN);
            while (pawns_can_en_passant) {
                int from_square = bitboard::bit_scan_forward(pawns_can_en_passant);
                move_list.emplace_back(from_square, board.get_en_passant_square(), PAWN, board.get_piece_from_square(board.get_en_passant_square()).type, EMPTY_PIECE, false, true, false);
                bitboard::pop_last_bit(pawns_can_en_passant);
            }
        }
    }

    /// @brief Generates all the pawn single push moves with promotion for the given board.
    /// @tparam ToMove The color to generate pawn single push moves with promotion for.
    /// @param move_list The list of moves to add the pawn single push moves with promotion to.
    /// @param board The board to generate pawn single push moves with promotion on.
    template<Color ToMove>
    static void generate_pawn_single_push_with_promotion(std::vector<Move> &move_list, const Board &board) noexcept {
        constexpr int PAWN_SINGLE_PUSH_OFFSET = ToMove == WHITE ? -8 : 8;
        constexpr u64 (*MASK_FUNC)(u64 wpawns, u64 empty) = ToMove == WHITE ? attacks::mask_white_pawn_single_pushes : attacks::mask_black_pawn_single_pushes;
        u64 pawn_single_pushes = MASK_FUNC(board.get_pieces(ToMove, PAWN), ~board.get_occupancies(BOTH));
        u64 pawn_single_pushes_promo = pawn_single_pushes & (utils::MASK_RANK[0] | utils::MASK_RANK[7]);
        while (pawn_single_pushes_promo) {
            int to_square = bitboard::bit_scan_forward(pawn_single_pushes_promo);
            int from_square = to_square + PAWN_SINGLE_PUSH_OFFSET;
            move_list.emplace_back(from_square, to_square, PAWN, EMPTY_PIECE, QUEEN, false, false, false);
            move_list.emplace_back(from_square, to_square, PAWN, EMPTY_PIECE, KNIGHT, false, false, false);
            move_list.emplace_back(from_square, to_square, PAWN, EMPTY_PIECE, ROOK, false, false, false);
            move_list.emplace_back(from_square, to_square, PAWN, EMPTY_PIECE, BISHOP, false, false, false);
            bitboard::pop_last_bit(pawn_single_pushes_promo);
        }
    }

    /// @brief Generates all the pawn single push moves without promotion for the given board.
    /// @tparam ToMove The color to generate pawn single push moves without promotion for.
    /// @param move_list The list of moves to add the pawn single push moves without promotion to.
    /// @param board The board to generate pawn single push moves without promotion on.
    template<Color ToMove>
    static void generate_pawn_single_push_no_promotion(std::vector<Move> &move_list, const Board &board) noexcept {
        constexpr int PAWN_SINGLE_PUSH_OFFSET = ToMove == WHITE ? -8 : 8;
        constexpr u64 (*MASK_FUNC)(u64 wpawns, u64 empty) = ToMove == WHITE ? attacks::mask_white_pawn_single_pushes : attacks::mask_black_pawn_single_pushes;
        u64 pawn_single_pushes = MASK_FUNC(board.get_pieces(ToMove, PAWN), ~board.get_occupancies(BOTH));
        u64 pawn_single_pushes_no_promo = pawn_single_pushes & utils::MASK_CLEAR_RANK[0] & utils::MASK_CLEAR_RANK[7];
        while (pawn_single_pushes_no_promo) {
            int to_square = bitboard::bit_scan_forward(pawn_single_pushes_no_promo);
            int from_square = to_square + PAWN_SINGLE_PUSH_OFFSET;
            move_list.emplace_back(from_square, to_square, PAWN, EMPTY_PIECE, EMPTY_PIECE, false, false, false);
            bitboard::pop_last_bit(pawn_single_pushes_no_promo);
        }
    }

    /// @brief Generates all the pawn capture moves with promotion for the given board.
    /// @tparam ToMove The color to generate pawn capture moves with promotion for.
    /// @param move_list The list of moves to add the pawn capture moves with promotion to.
    /// @param board The board to generate pawn capture moves with promotion on.
    template<Color ToMove>
    static void generate_pawn_captures_with_promotion(std::vector<Move> &move_list, const Board &board) {
        constexpr Color Opponent = ToMove == WHITE ? BLACK : WHITE;
        constexpr int MASK_INDEX = ToMove == WHITE ? 6 : 1;
        u64 pawns_can_capture_with_promo = board.get_pieces(ToMove, PAWN) & utils::MASK_RANK[MASK_INDEX];
        while (pawns_can_capture_with_promo) {
            Square from_square = bitboard::bit_scan_forward(pawns_can_capture_with_promo);
            u64 pawn_captures_promo = attacks::ATTACKS_PAWN[ToMove][from_square] & board.get_occupancies(Opponent);
            while (pawn_captures_promo) {
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

    /// @brief Generates all the pawn capture moves without promotion for the given board.
    /// @tparam ToMove The color to generate pawn capture moves without promotion for.
    /// @param move_list The list of moves to add the pawn capture moves without promotion to.
    /// @param board The board to generate pawn capture moves without promotion on.
    template<Color ToMove>
    static void generate_pawn_captures_no_promotion(std::vector<Move> &move_list, const Board &board) noexcept {
        constexpr Color Opponent = ToMove == WHITE ? BLACK : WHITE;
        constexpr int MASK_INDEX = ToMove == WHITE ? 6 : 1;
        u64 pawns_can_capture_no_promo = board.get_pieces(ToMove, PAWN) & utils::MASK_CLEAR_RANK[MASK_INDEX];
        while (pawns_can_capture_no_promo) {
            Square from_square = bitboard::bit_scan_forward(pawns_can_capture_no_promo);
            u64 pawn_captures_no_promo = attacks::ATTACKS_PAWN[ToMove][from_square] & board.get_occupancies(Opponent);
            while (pawn_captures_no_promo) {
                Square to_square = bitboard::bit_scan_forward(pawn_captures_no_promo);
                move_list.emplace_back(from_square, to_square, PAWN, board.get_piece_from_square(to_square).type, EMPTY_PIECE, false, false, false);
                bitboard::pop_last_bit(pawn_captures_no_promo);
            }
            bitboard::pop_last_bit(pawns_can_capture_no_promo);
        }
    }

    /// @brief Generates all the pawn double push moves for the given board.
    /// @tparam ToMove The color to generate pawn double push moves for.
    /// @param move_list The list of moves to add the pawn double push moves to.
    /// @param board The board to generate pawn double push moves on.
    template<Color ToMove>
    static void generate_pawn_double_pushes(std::vector<Move> &move_list, const Board &board) noexcept {
        constexpr int PAWN_DOUBLE_PUSH_OFFSET = ToMove == WHITE ? -16 : 16;
        constexpr u64 (*MASK_FUNC)(u64 wpawns, u64 empty) = ToMove == WHITE ? attacks::mask_white_pawn_double_pushes : attacks::mask_black_pawn_double_pushes;
        u64 pawn_double_pushes = MASK_FUNC(board.get_pieces(ToMove, PAWN), ~board.get_occupancies(BOTH));
        while (pawn_double_pushes) {
            int to_square = bitboard::bit_scan_forward(pawn_double_pushes);
            int from_square = to_square + PAWN_DOUBLE_PUSH_OFFSET;
            move_list.emplace_back(from_square, to_square, PAWN, EMPTY_PIECE, EMPTY_PIECE, true, false, false);
            bitboard::pop_last_bit(pawn_double_pushes);
        }
    }

    /// @brief Generates all the leaper moves for the given board.
    /// @tparam ToMove The color to generate leaper moves for.
    /// @tparam PType The type of piece to generate leaper moves for.
    /// @tparam GType The type of move generation to perform.
    /// @param move_list The list of moves to add the leaper moves to.
    /// @param board The board to generate leaper moves on.
    template<Color ToMove, PieceType PType, GenType GType>
    static void generate_leaper_moves(std::vector<Move> &move_list, const Board &board) noexcept {
        static_assert(PType == KNIGHT || PType == KING, "Unsupported piece type in generateLeaperMoves()");

        constexpr Color Opponent = ToMove == WHITE ? BLACK : WHITE;

        u64 to_move_pieces = board.get_pieces(ToMove, PType);
        u64 to_move_occupancies = board.get_occupancies(ToMove);
        u64 opponent_occupancies = board.get_occupancies(Opponent);

        while (to_move_pieces) {
            Square from_square = bitboard::bit_scan_forward(to_move_pieces);
            u64 moves;
            if constexpr (GType == QUIETS) {
                if constexpr (PType == KNIGHT) {
                    moves = attacks::ATTACKS_KNIGHT[from_square] & ~to_move_occupancies;
                } else {
                    moves = attacks::ATTACKS_KING[from_square] & ~to_move_occupancies;
                }
            } else {
                if constexpr (PType == KNIGHT) {
                    moves = attacks::ATTACKS_KNIGHT[from_square] & ~to_move_occupancies & opponent_occupancies;
                } else {
                    moves = attacks::ATTACKS_KING[from_square] & ~to_move_occupancies & opponent_occupancies;
                }
            }
            while (moves) {
                Square to_square = bitboard::bit_scan_forward(moves);
                if constexpr (GType == QUIETS) {
                    move_list.emplace_back(from_square, to_square, PType, EMPTY_PIECE, EMPTY_PIECE, false, false, false);
                } else {
                    move_list.emplace_back(from_square, to_square, PType, board.get_piece_from_square(to_square).type, EMPTY_PIECE, false, false, false);
                }
                bitboard::pop_last_bit(moves);
            }
            bitboard::pop_last_bit(to_move_pieces);
        }
    }

    /// @brief Generates all the slider moves for the given board.
    /// @tparam ToMove The color to generate slider moves for.
    /// @tparam PType The type of piece to generate slider moves for.
    /// @tparam GType The type of move generation to perform.
    /// @param move_list The list of moves to add the slider moves to.
    /// @param board The board to generate slider moves on.
    template<Color ToMove, PieceType PType, GenType GType>
    static void generate_slider_moves(std::vector<Move> &move_list, const Board &board) {
        static_assert(PType == BISHOP || PType == ROOK || PType == QUEEN, "Unsupported piece type in generate_slider_moves()");

        constexpr Color Opponent = ToMove == WHITE ? BLACK : WHITE;
        constexpr u64 (*ATTACKS_FUNC)(const Square sq, u64 occ) = (PType == BISHOP ? attacks::get_bishop_attacks
                                                                   : PType == ROOK ? attacks::get_rook_attacks
                                                                                   : attacks::get_queen_attacks);
        u64 to_move_occupancies = board.get_occupancies(ToMove);
        u64 opponent_occupancies = board.get_occupancies(Opponent);
        u64 to_move_pieces = board.get_pieces(ToMove, PType);
        while (to_move_pieces) {
            Square from_square = bitboard::bit_scan_forward(to_move_pieces);
            u64 moves;
            if constexpr (GType == QUIETS) {
                moves = ATTACKS_FUNC(from_square, board.get_occupancies(BOTH)) & ~to_move_occupancies & ~opponent_occupancies;
            } else {
                moves = ATTACKS_FUNC(from_square, board.get_occupancies(BOTH)) & ~to_move_occupancies & opponent_occupancies;
            }
            while (moves) {
                Square to_square = bitboard::bit_scan_forward(moves);
                if constexpr (GType == QUIETS) {
                    move_list.emplace_back(from_square, to_square, PType, EMPTY_PIECE, EMPTY_PIECE, false, false, false);
                } else {
                    move_list.emplace_back(from_square, to_square, PType, board.get_piece_from_square(to_square).type, EMPTY_PIECE, false, false, false);
                }
                bitboard::pop_last_bit(moves);
            }
            bitboard::pop_last_bit(to_move_pieces);
        }
    }

    bool has_legal_moves(Board &board) noexcept {
        Board::GameState state = board.get_state();
        for (const Move &move: movegen::generate_pseudo_legal_moves(board)) {
            board.make(move);
            Square king_sq = bitboard::bit_scan_forward(board.get_pieces(board.get_opponent(), KING));
            if (!board.is_square_attacked(king_sq, board.get_side_to_move())) {
                board.unmake(move, state);
                return true;
            }
            board.unmake(move, state);
        }
        return false;
    }

    std::vector<Move> generate_pseudo_legal_moves(const Board &board) noexcept {
        std::vector<Move> moves;
        moves.reserve(MAX_SIZE_MOVES_ARRAY);

        if (board.get_side_to_move() == WHITE) {
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
        } else {
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

    std::vector<Move> generate_pseudo_legal_captures(const Board &board) noexcept {
        std::vector<Move> moves;
        moves.reserve(MAX_SIZE_MOVES_ARRAY);

        if (board.get_side_to_move() == WHITE) {
            generate_pawn_captures_with_promotion<WHITE>(moves, board);
            generate_pawn_captures_no_promotion<WHITE>(moves, board);
            generate_en_passant_capture<WHITE>(moves, board);

            generate_leaper_moves<WHITE, KNIGHT, CAPTURES>(moves, board);
            generate_leaper_moves<WHITE, KING, CAPTURES>(moves, board);

            generate_slider_moves<WHITE, BISHOP, CAPTURES>(moves, board);
            generate_slider_moves<WHITE, ROOK, CAPTURES>(moves, board);
            generate_slider_moves<WHITE, QUEEN, CAPTURES>(moves, board);
        } else {
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

    std::vector<Move> generate_legal_moves(Board &board) noexcept {
        std::vector<Move> moves;
        moves.reserve(MAX_SIZE_MOVES_ARRAY);

        Board::GameState state = board.get_state();
        for (const Move &move: movegen::generate_pseudo_legal_moves(board)) {
            board.make(move);
            Color attacker_side = board.get_side_to_move();
            Square king_sq = bitboard::bit_scan_forward(board.get_pieces(board.get_opponent(), KING));
            if (!board.is_square_attacked(king_sq, attacker_side)) {
                moves.push_back(move);
            }
            board.unmake(move, state);
        }
        return moves;
    }

    std::vector<Move> generate_legal_captures(Board &board) noexcept {
        std::vector<Move> captures;
        captures.reserve(MAX_SIZE_MOVES_ARRAY);

        Board::GameState state = board.get_state();
        for (const Move &move: movegen::generate_pseudo_legal_captures(board)) {
            board.make(move);
            Color attacker_side = board.get_side_to_move();
            Square king_sq = bitboard::bit_scan_forward(board.get_pieces(board.get_opponent(), KING));
            if (!board.is_square_attacked(king_sq, attacker_side)) {
                captures.push_back(move);
            }
            board.unmake(move, state);
        }
        return captures;
    }
}// namespace movegen