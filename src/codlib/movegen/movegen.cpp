#include "codlib/movegen.hpp"

#include <codlib/movegen/attacks.hpp>
#include <codlib/bitboard.hpp>
#include <codlib/board.hpp>
#include <codlib/move.hpp>

#define MAX_SIZE_MOVES_ARRAY 256

using bitboard::Bitboard;

namespace movegen {
    /// @brief The type of move generation to perform.
    enum GenType {
        /// @brief Generate capture moves.
        CAPTURES, /// @brief Generate quiet moves (non-captures).
        QUIETS,
    };

    /// @brief Generates all the castling_availability moves for the given board.
    /// @tparam ToMove The color to generate castling_availability moves for.
    /// @param move_list The list of moves to add the castling_availability moves to.
    /// @param board The board to generate castling_availability moves on.
    template<Color ToMove>
    static void CastlingMoves(std::vector<Move> &move_list, const Board &board) noexcept {
        constexpr Color Opponent = (ToMove == WHITE ? BLACK : WHITE);
        constexpr Square CASTLE_B_SQ = (ToMove == WHITE ? B1 : B8);
        constexpr Square CASTLE_C_SQ = (ToMove == WHITE ? C1 : C8);
        constexpr Square CASTLE_D_SQ = (ToMove == WHITE ? D1 : D8);
        constexpr Square CASTLE_E_SQ = (ToMove == WHITE ? E1 : E8);
        constexpr Square CASTLE_F_SQ = (ToMove == WHITE ? F1 : F8);
        constexpr Square CASTLE_G_SQ = (ToMove == WHITE ? G1 : G8);
        constexpr CastlingAvailability CASTLE_KING_MASK = (ToMove == WHITE ? WHITE_KING : BLACK_KING);
        constexpr CastlingAvailability CASTLE_QUEEN_MASK = (ToMove == WHITE ? WHITE_QUEEN : BLACK_QUEEN);
        if (!board.IsSquareAttacked(CASTLE_E_SQ, Opponent)) {
            if ((board.castling_availability() & CASTLE_KING_MASK) &&
                !bitboard::GetBit(board.occupancies(BOTH), CASTLE_F_SQ) &&
                !bitboard::GetBit(board.occupancies(BOTH), CASTLE_G_SQ)) {
                if (!board.IsSquareAttacked(CASTLE_F_SQ, Opponent) && !board.IsSquareAttacked(CASTLE_G_SQ, Opponent)) {
                    move_list.emplace_back(CASTLE_E_SQ, CASTLE_G_SQ, KING, EMPTY_PIECE, EMPTY_PIECE, false, false,
                                           true);
                }
            }
            if ((board.castling_availability() & CASTLE_QUEEN_MASK) &&
                !bitboard::GetBit(board.occupancies(BOTH), CASTLE_D_SQ) &&
                !bitboard::GetBit(board.occupancies(BOTH), CASTLE_C_SQ) &&
                !bitboard::GetBit(board.occupancies(BOTH), CASTLE_B_SQ)) {
                if (!board.IsSquareAttacked(CASTLE_D_SQ, Opponent) && !board.IsSquareAttacked(CASTLE_C_SQ, Opponent)) {
                    move_list.emplace_back(CASTLE_E_SQ, CASTLE_C_SQ, KING, EMPTY_PIECE, EMPTY_PIECE, false, false,
                                           true);
                }
            }
        }
    }

    /// @brief Generates all the en passant capture moves for the given board.
    /// @tparam ToMove The color to generate en passant capture moves for.
    /// @param move_list The list of moves to add the en passant capture moves to.
    /// @param board The board to generate en passant capture moves on.
    template<Color ToMove>
    static void EnPassantCaptures(std::vector<Move> &move_list, const Board &board) noexcept {
        constexpr Color Opponent = ToMove == WHITE ? BLACK : WHITE;
        if (board.en_passant_square() != -1) {
            Bitboard pawns_can_en_passant =
                    attacks::PAWN_ATTACKS[Opponent][board.en_passant_square()] & board.pieces(ToMove, PAWN);
            while (pawns_can_en_passant) {
                int from_square = bitboard::BitScanForward(pawns_can_en_passant);
                move_list.emplace_back(from_square, board.en_passant_square(), PAWN,
                                       board.piece(board.en_passant_square()).type, EMPTY_PIECE, false, true, false);
                bitboard::PopLastBit(pawns_can_en_passant);
            }
        }
    }

    /// @brief Generates all the pawn single push moves with promotion for the given board.
    /// @tparam ToMove The color to generate pawn single push moves with promotion for.
    /// @param move_list The list of moves to add the pawn single push moves with promotion to.
    /// @param board The board to generate pawn single push moves with promotion on.
    template<Color ToMove>
    static void PawnSinglePushPromotions(std::vector<Move> &move_list, const Board &board) noexcept {
        constexpr int PAWN_SINGLE_PUSH_OFFSET = ToMove == WHITE ? -8 : 8;
        constexpr Bitboard (*MASK_FUNC)(Bitboard wpawns, Bitboard empty) = attacks::PawnSinglePushes<ToMove>;

        const Bitboard pawn_single_pushes = MASK_FUNC(board.pieces(ToMove, PAWN), ~board.occupancies(BOTH));
        Bitboard pawn_single_pushes_promo = pawn_single_pushes & (utils::MASK_RANK[0] | utils::MASK_RANK[7]);
        while (pawn_single_pushes_promo) {
            int to_square = bitboard::BitScanForward(pawn_single_pushes_promo);
            int from_square = to_square + PAWN_SINGLE_PUSH_OFFSET;
            move_list.emplace_back(from_square, to_square, PAWN, EMPTY_PIECE, QUEEN, false, false, false);
            move_list.emplace_back(from_square, to_square, PAWN, EMPTY_PIECE, KNIGHT, false, false, false);
            move_list.emplace_back(from_square, to_square, PAWN, EMPTY_PIECE, ROOK, false, false, false);
            move_list.emplace_back(from_square, to_square, PAWN, EMPTY_PIECE, BISHOP, false, false, false);
            bitboard::PopLastBit(pawn_single_pushes_promo);
        }
    }

    /// @brief Generates all the pawn single push moves without promotion for the given board.
    /// @tparam ToMove The color to generate pawn single push moves without promotion for.
    /// @param move_list The list of moves to add the pawn single push moves without promotion to.
    /// @param board The board to generate pawn single push moves without promotion on.
    template<Color ToMove>
    static void PawnSinglePushesNoPromotion(std::vector<Move> &move_list, const Board &board) noexcept {
        constexpr int PAWN_SINGLE_PUSH_OFFSET = ToMove == WHITE ? -8 : 8;
        constexpr Bitboard (*MASK_FUNC)(Bitboard wpawns, Bitboard empty) = attacks::PawnSinglePushes<ToMove>;

        const Bitboard pawn_single_pushes = MASK_FUNC(board.pieces(ToMove, PAWN), ~board.occupancies(BOTH));
        Bitboard pawn_single_pushes_no_promo =
                pawn_single_pushes & utils::MASK_CLEAR_RANK[0] & utils::MASK_CLEAR_RANK[7];
        while (pawn_single_pushes_no_promo) {
            int to_square = bitboard::BitScanForward(pawn_single_pushes_no_promo);
            int from_square = to_square + PAWN_SINGLE_PUSH_OFFSET;
            move_list.emplace_back(from_square, to_square, PAWN, EMPTY_PIECE, EMPTY_PIECE, false, false, false);
            bitboard::PopLastBit(pawn_single_pushes_no_promo);
        }
    }

    /// @brief Generates all the pawn capture moves with promotion for the given board.
    /// @tparam ToMove The color to generate pawn capture moves with promotion for.
    /// @param move_list The list of moves to add the pawn capture moves with promotion to.
    /// @param board The board to generate pawn capture moves with promotion on.
    template<Color ToMove>
    static void PawnCapturesPromotion(std::vector<Move> &move_list, const Board &board) {
        constexpr Color Opponent = ToMove == WHITE ? BLACK : WHITE;
        constexpr int MASK_INDEX = ToMove == WHITE ? 6 : 1;

        Bitboard pawns_can_capture_with_promo = board.pieces(ToMove, PAWN) & utils::MASK_RANK[MASK_INDEX];
        while (pawns_can_capture_with_promo) {
            const Square from_square = bitboard::BitScanForward(pawns_can_capture_with_promo);
            Bitboard pawn_captures_promo = attacks::PAWN_ATTACKS[ToMove][from_square] & board.occupancies(Opponent);
            while (pawn_captures_promo) {
                Square to_square = bitboard::BitScanForward(pawn_captures_promo);
                move_list.emplace_back(from_square, to_square, PAWN, board.piece(to_square).type, QUEEN, false, false,
                                       false);
                move_list.emplace_back(from_square, to_square, PAWN, board.piece(to_square).type, KNIGHT, false, false,
                                       false);
                move_list.emplace_back(from_square, to_square, PAWN, board.piece(to_square).type, ROOK, false, false,
                                       false);
                move_list.emplace_back(from_square, to_square, PAWN, board.piece(to_square).type, BISHOP, false, false,
                                       false);
                bitboard::PopLastBit(pawn_captures_promo);
            }
            bitboard::PopLastBit(pawns_can_capture_with_promo);
        }
    }

    /// @brief Generates all the pawn capture moves without promotion for the given board.
    /// @tparam ToMove The color to generate pawn capture moves without promotion for.
    /// @param move_list The list of moves to add the pawn capture moves without promotion to.
    /// @param board The board to generate pawn capture moves without promotion on.
    template<Color ToMove>
    static void PawnCapturesNoPromotion(std::vector<Move> &move_list, const Board &board) noexcept {
        constexpr Color Opponent = ToMove == WHITE ? BLACK : WHITE;
        constexpr int MASK_INDEX = ToMove == WHITE ? 6 : 1;

        Bitboard pawns_can_capture_no_promo = board.pieces(ToMove, PAWN) & utils::MASK_CLEAR_RANK[MASK_INDEX];
        while (pawns_can_capture_no_promo) {
            Square from_square = bitboard::BitScanForward(pawns_can_capture_no_promo);
            Bitboard pawn_captures_no_promo = attacks::PAWN_ATTACKS[ToMove][from_square] & board.occupancies(Opponent);
            while (pawn_captures_no_promo) {
                Square to_square = bitboard::BitScanForward(pawn_captures_no_promo);
                move_list.emplace_back(from_square, to_square, PAWN, board.piece(to_square).type, EMPTY_PIECE, false,
                                       false, false);
                bitboard::PopLastBit(pawn_captures_no_promo);
            }
            bitboard::PopLastBit(pawns_can_capture_no_promo);
        }
    }

    /// @brief Generates all the pawn double push moves for the given board.
    /// @tparam ToMove The color to generate pawn double push moves for.
    /// @param move_list The list of moves to add the pawn double push moves to.
    /// @param board The board to generate pawn double push moves on.
    template<Color ToMove>
    static void PawnDoublePushes(std::vector<Move> &move_list, const Board &board) noexcept {
        constexpr int PAWN_DOUBLE_PUSH_OFFSET = ToMove == WHITE ? -16 : 16;
        constexpr Bitboard (*MASK_FUNC)(Bitboard wpawns, Bitboard empty) = attacks::PawnDoublePushes<ToMove>;

        Bitboard pawn_double_pushes = MASK_FUNC(board.pieces(ToMove, PAWN), ~board.occupancies(BOTH));
        while (pawn_double_pushes) {
            int to_square = bitboard::BitScanForward(pawn_double_pushes);
            int from_square = to_square + PAWN_DOUBLE_PUSH_OFFSET;
            move_list.emplace_back(from_square, to_square, PAWN, EMPTY_PIECE, EMPTY_PIECE, true, false, false);
            bitboard::PopLastBit(pawn_double_pushes);
        }
    }

    /// @brief Generates all the leaper moves for the given board.
    /// @tparam ToMove The color to generate leaper moves for.
    /// @tparam PType The type of piece to generate leaper moves for.
    /// @tparam GType The type of move generation to perform.
    /// @param move_list The list of moves to add the leaper moves to.
    /// @param board The board to generate leaper moves on.
    template<Color ToMove, PieceType PType, GenType GType>
    static void LeaperMoves(std::vector<Move> &move_list, const Board &board) noexcept {
        static_assert(PType == KNIGHT || PType == KING, "Unsupported piece type in generateLeaperMoves()");

        constexpr Color Opponent = ToMove == WHITE ? BLACK : WHITE;

        Bitboard to_move_pieces = board.pieces(ToMove, PType);
        const Bitboard to_move_occupancies = board.occupancies(ToMove);
        const Bitboard opponent_occupancies = board.occupancies(Opponent);

        while (to_move_pieces) {
            Square from_square = bitboard::BitScanForward(to_move_pieces);
            Bitboard moves;
            if constexpr (GType == QUIETS) {
                if constexpr (PType == KNIGHT) {
                    moves = attacks::KNIGHT_ATTACKS[from_square] & ~to_move_occupancies;
                } else {
                    moves = attacks::KING_ATTACKS[from_square] & ~to_move_occupancies;
                }
            } else {
                if constexpr (PType == KNIGHT) {
                    moves = attacks::KNIGHT_ATTACKS[from_square] & ~to_move_occupancies & opponent_occupancies;
                } else {
                    moves = attacks::KING_ATTACKS[from_square] & ~to_move_occupancies & opponent_occupancies;
                }
            }
            while (moves) {
                Square to_square = bitboard::BitScanForward(moves);
                if constexpr (GType == QUIETS) {
                    move_list.emplace_back(from_square, to_square, PType, EMPTY_PIECE, EMPTY_PIECE, false, false,
                                           false);
                } else {
                    move_list.emplace_back(from_square, to_square, PType, board.piece(to_square).type, EMPTY_PIECE,
                                           false, false, false);
                }
                bitboard::PopLastBit(moves);
            }
            bitboard::PopLastBit(to_move_pieces);
        }
    }

    /// @brief Generates all the slider moves for the given board.
    /// @tparam ToMove The color to generate slider moves for.
    /// @tparam PType The type of piece to generate slider moves for.
    /// @tparam GType The type of move generation to perform.
    /// @param move_list The list of moves to add the slider moves to.
    /// @param board The board to generate slider moves on.
    template<Color ToMove, PieceType PType, GenType GType>
    static void SliderMoves(std::vector<Move> &move_list, const Board &board) {
        static_assert(PType == BISHOP || PType == ROOK || PType == QUEEN, "Unsupported piece type in SliderMoves()");

        constexpr Color Opponent = ToMove == WHITE ? BLACK : WHITE;
        constexpr Bitboard
        (*ATTACKS_FUNC)(const Square sq, Bitboard occ) = (PType == BISHOP ? attacks::BishopAttacks : PType == ROOK
                                                                                                     ? attacks::RookAttacks
                                                                                                     : attacks::QueenAttacks);
        const Bitboard to_move_occupancies = board.occupancies(ToMove);
        const Bitboard opponent_occupancies = board.occupancies(Opponent);
        Bitboard to_move_pieces = board.pieces(ToMove, PType);
        while (to_move_pieces) {
            Square from_square = bitboard::BitScanForward(to_move_pieces);
            Bitboard moves;
            if constexpr (GType == QUIETS) {
                moves = ATTACKS_FUNC(from_square, board.occupancies(BOTH)) & ~to_move_occupancies &
                        ~opponent_occupancies;
            } else {
                moves = ATTACKS_FUNC(from_square, board.occupancies(BOTH)) & ~to_move_occupancies &
                        opponent_occupancies;
            }
            while (moves) {
                Square to_square = bitboard::BitScanForward(moves);
                if constexpr (GType == QUIETS) {
                    move_list.emplace_back(from_square, to_square, PType, EMPTY_PIECE, EMPTY_PIECE, false, false,
                                           false);
                } else {
                    move_list.emplace_back(from_square, to_square, PType, board.piece(to_square).type, EMPTY_PIECE,
                                           false, false, false);
                }
                bitboard::PopLastBit(moves);
            }
            bitboard::PopLastBit(to_move_pieces);
        }
    }

    [[maybe_unused]] std::vector<Move> PseudoLegal(const Board &board) noexcept {
        std::vector<Move> moves;
        moves.reserve(MAX_SIZE_MOVES_ARRAY);

        if (board.active() == WHITE) {
            PawnCapturesPromotion<WHITE>(moves, board);
            PawnCapturesNoPromotion<WHITE>(moves, board);
            EnPassantCaptures<WHITE>(moves, board);
            PawnSinglePushPromotions<WHITE>(moves, board);
            PawnDoublePushes<WHITE>(moves, board);
            PawnSinglePushesNoPromotion<WHITE>(moves, board);

            LeaperMoves<WHITE, KNIGHT, CAPTURES>(moves, board);
            LeaperMoves<WHITE, KING, CAPTURES>(moves, board);
            SliderMoves<WHITE, BISHOP, CAPTURES>(moves, board);
            SliderMoves<WHITE, ROOK, CAPTURES>(moves, board);
            SliderMoves<WHITE, QUEEN, CAPTURES>(moves, board);

            LeaperMoves<WHITE, KNIGHT, QUIETS>(moves, board);
            LeaperMoves<WHITE, KING, QUIETS>(moves, board);
            SliderMoves<WHITE, BISHOP, QUIETS>(moves, board);
            SliderMoves<WHITE, ROOK, QUIETS>(moves, board);
            SliderMoves<WHITE, QUEEN, QUIETS>(moves, board);

            CastlingMoves<WHITE>(moves, board);
        } else {
            PawnCapturesPromotion<BLACK>(moves, board);
            PawnCapturesNoPromotion<BLACK>(moves, board);
            EnPassantCaptures<BLACK>(moves, board);
            PawnSinglePushPromotions<BLACK>(moves, board);
            PawnDoublePushes<BLACK>(moves, board);
            PawnSinglePushesNoPromotion<BLACK>(moves, board);

            LeaperMoves<BLACK, KNIGHT, CAPTURES>(moves, board);
            LeaperMoves<BLACK, KING, CAPTURES>(moves, board);
            SliderMoves<BLACK, BISHOP, CAPTURES>(moves, board);
            SliderMoves<BLACK, ROOK, CAPTURES>(moves, board);
            SliderMoves<BLACK, QUEEN, CAPTURES>(moves, board);

            LeaperMoves<BLACK, KNIGHT, QUIETS>(moves, board);
            LeaperMoves<BLACK, KING, QUIETS>(moves, board);
            SliderMoves<BLACK, BISHOP, QUIETS>(moves, board);
            SliderMoves<BLACK, ROOK, QUIETS>(moves, board);
            SliderMoves<BLACK, QUEEN, QUIETS>(moves, board);

            CastlingMoves<BLACK>(moves, board);
        }

        return moves;
    }

    [[maybe_unused]] std::vector<Move> PseudoLegalCaptures(const Board &board) noexcept {
        std::vector<Move> moves;
        moves.reserve(MAX_SIZE_MOVES_ARRAY);

        if (board.active() == WHITE) {
            PawnCapturesPromotion<WHITE>(moves, board);
            PawnCapturesNoPromotion<WHITE>(moves, board);
            EnPassantCaptures<WHITE>(moves, board);

            LeaperMoves<WHITE, KNIGHT, CAPTURES>(moves, board);
            LeaperMoves<WHITE, KING, CAPTURES>(moves, board);

            SliderMoves<WHITE, BISHOP, CAPTURES>(moves, board);
            SliderMoves<WHITE, ROOK, CAPTURES>(moves, board);
            SliderMoves<WHITE, QUEEN, CAPTURES>(moves, board);
        } else {
            PawnCapturesPromotion<BLACK>(moves, board);
            PawnCapturesNoPromotion<BLACK>(moves, board);
            EnPassantCaptures<BLACK>(moves, board);

            LeaperMoves<BLACK, KNIGHT, CAPTURES>(moves, board);
            LeaperMoves<BLACK, KING, CAPTURES>(moves, board);

            SliderMoves<BLACK, BISHOP, CAPTURES>(moves, board);
            SliderMoves<BLACK, ROOK, CAPTURES>(moves, board);
            SliderMoves<BLACK, QUEEN, CAPTURES>(moves, board);
        }

        return moves;
    }

    [[maybe_unused]] bool HasLegalMoves(Board &board) noexcept {
        const Board::StateBackup state = board.GetStateBackup();
        for (const Move &move: PseudoLegal(board)) {
            board.Make(move);
            Square king_sq = bitboard::BitScanForward(board.pieces(board.inactive(), KING));
            if (!board.IsSquareAttacked(king_sq, board.active())) {
                board.Unmake(move, state);
                return true;
            }
            board.Unmake(move, state);
        }
        return false;
    }

    [[maybe_unused]] std::vector<Move> LegalMoves(Board &board) noexcept {
        std::vector<Move> moves;
        moves.reserve(MAX_SIZE_MOVES_ARRAY);

        const Board::StateBackup state = board.GetStateBackup();
        for (const Move &move: movegen::PseudoLegal(board)) {
            board.Make(move);
            Color attacker_side = board.active();
            Square king_sq = bitboard::BitScanForward(board.pieces(board.inactive(), KING));
            if (!board.IsSquareAttacked(king_sq, attacker_side)) { moves.push_back(move); }
            board.Unmake(move, state);
        }
        return moves;
    }

    [[maybe_unused]] std::vector<Move> LegalCaptures(Board &board) noexcept {
        std::vector<Move> captures;
        captures.reserve(MAX_SIZE_MOVES_ARRAY);

        const Board::StateBackup state = board.GetStateBackup();
        for (const Move &move: movegen::PseudoLegalCaptures(board)) {
            board.Make(move);
            Color attacker_side = board.active();
            Square king_sq = bitboard::BitScanForward(board.pieces(board.inactive(), KING));
            if (!board.IsSquareAttacked(king_sq, attacker_side)) { captures.push_back(move); }
            board.Unmake(move, state);
        }
        return captures;
    }
}// namespace movegen