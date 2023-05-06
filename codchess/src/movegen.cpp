#include <codchess/movegen.hpp>

#include <codchess/bitboard.hpp>

#include "attacks.hpp"

namespace codchess::movegen {
/// @brief The type of move generation to perform.
enum GenType {
    /// @brief Generate capture moves.
    CAPTURES,
    /// @brief Generate quiet moves (non-captures).
    QUIETS,
};

/// @brief Generates all the castling_availability moves for the given board.
/// @tparam ToMove The color to generate castling_availability moves for.
/// @param move_list The list of moves to add the castling_availability moves
/// to.
/// @param board The board to generate castling_availability moves on.
template <Color ToMove>
static void
CastlingMoves(MoveList &move_list, const Board &board) noexcept {
    static_assert(ToMove == WHITE or ToMove == BLACK, "Invalid color");

    constexpr auto Opponent = (ToMove == WHITE ? BLACK : WHITE);

    constexpr auto CastleBSq = (ToMove == WHITE ? B1 : B8);
    constexpr auto CastleCSq = (ToMove == WHITE ? C1 : C8);
    constexpr auto CastleDSq = (ToMove == WHITE ? D1 : D8);
    constexpr auto CastleESq = (ToMove == WHITE ? E1 : E8);
    constexpr auto CastleFSq = (ToMove == WHITE ? F1 : F8);
    constexpr auto CastleGSq = (ToMove == WHITE ? G1 : G8);

    constexpr auto CastleKingMask = (ToMove == WHITE ? WHITE_KING : BLACK_KING);
    constexpr auto CastleQueenMask =
        (ToMove == WHITE ? WHITE_QUEEN : BLACK_QUEEN);

    if (!board.IsSquareAttacked(CastleESq, Opponent)) {
        if ((board.castling_availability() & CastleKingMask) &&
            !bitboard::GetBit(board.occupancies(BOTH), CastleFSq) &&
            !bitboard::GetBit(board.occupancies(BOTH), CastleGSq)) {
            if (!board.IsSquareAttacked(CastleFSq, Opponent) &&
                !board.IsSquareAttacked(CastleGSq, Opponent)) {

                move_list.Insert({CastleESq, CastleGSq, KING, EMPTY_PIECE,
                                  EMPTY_PIECE, false, false, true});
            }
        }
        if ((board.castling_availability() & CastleQueenMask) &&
            !bitboard::GetBit(board.occupancies(BOTH), CastleDSq) &&
            !bitboard::GetBit(board.occupancies(BOTH), CastleCSq) &&
            !bitboard::GetBit(board.occupancies(BOTH), CastleBSq)) {
            if (!board.IsSquareAttacked(CastleDSq, Opponent) &&
                !board.IsSquareAttacked(CastleCSq, Opponent)) {

                move_list.Insert({CastleESq, CastleCSq, KING, EMPTY_PIECE,
                                  EMPTY_PIECE, false, false, true});
            }
        }
    }
}

/// @brief Generates all the en passant capture moves for the given board.
/// @tparam ToMove The color to generate en passant capture moves for.
/// @param move_list The list of moves to add the en passant capture moves to.
/// @param board The board to generate en passant capture moves on.
template <Color ToMove>
static void
EnPassantCaptures(MoveList &move_list, const Board &board) noexcept {
    static_assert(ToMove == WHITE or ToMove == BLACK, "Invalid color");

    constexpr auto Opponent = ToMove == WHITE ? BLACK : WHITE;

    if (board.en_passant_square() != EMPTY_SQUARE) {
        auto pawns_can_en_passant =
            attacks::PAWN_ATTACKS[Opponent][board.en_passant_square()] &
            board.pieces(ToMove, PAWN);

        while (pawns_can_en_passant) {
            const auto from_square =
                bitboard::BitScanForward(pawns_can_en_passant);

            move_list.Insert({from_square, board.en_passant_square(), PAWN,
                              board.piece(board.en_passant_square()).type,
                              EMPTY_PIECE, false, true, false});

            bitboard::PopLastBit(pawns_can_en_passant);
        }
    }
}

/// @brief Generates all the pawn single push moves with promotion
/// @tparam ToMove The color for which to generate the moves
/// @param move_list The list of moves to add the pawn single push moves with
/// promotion to.
/// @param board The board to generate pawn single push moves with promotion on.
template <Color ToMove>
static void
PawnSinglePushPromotions(MoveList &move_list, const Board &board) noexcept {
    static_assert(ToMove == WHITE or ToMove == BLACK, "Invalid color");

    constexpr auto PawnSinglePushOffset = ToMove == WHITE ? -8 : 8;

    auto pawn_single_pushes_promo =
        attacks::PawnSinglePushes<ToMove>(board.pieces(ToMove, PAWN),
                                          ~board.occupancies(BOTH)) &
        (utils::MASK_RANK[0] | utils::MASK_RANK[7]);

    while (pawn_single_pushes_promo) {
        const auto to_square =
            bitboard::BitScanForward(pawn_single_pushes_promo);
        const auto from_square = to_square + PawnSinglePushOffset;

        move_list.Insert({from_square, to_square, PAWN, EMPTY_PIECE, QUEEN,
                          false, false, false});

        move_list.Insert({from_square, to_square, PAWN, EMPTY_PIECE, KNIGHT,
                          false, false, false});

        move_list.Insert({from_square, to_square, PAWN, EMPTY_PIECE, ROOK,
                          false, false, false});

        move_list.Insert({from_square, to_square, PAWN, EMPTY_PIECE, BISHOP,
                          false, false, false});

        bitboard::PopLastBit(pawn_single_pushes_promo);
    }
}

/// @brief Generates all the pawn single push moves without promotion for the
/// given board.
/// @tparam ToMove The color to generate pawn single push moves without
/// promotion for.
/// @param move_list The list of moves to add the pawn single push moves without
/// promotion to.
/// @param board The board to generate pawn single push moves without promotion
/// on.
template <Color ToMove>
static void
PawnSinglePushesNoPromotion(MoveList &move_list, const Board &board) noexcept {
    static_assert(ToMove == WHITE or ToMove == BLACK, "Invalid color");

    constexpr auto PawnSinglePushOffset = ToMove == WHITE ? -8 : 8;

    auto pawn_single_pushes_no_promo =
        attacks::PawnSinglePushes<ToMove>(board.pieces(ToMove, PAWN),
                                          ~board.occupancies(BOTH)) &
        utils::MASK_CLEAR_RANK[0] & utils::MASK_CLEAR_RANK[7];

    while (pawn_single_pushes_no_promo) {
        const auto to_square =
            bitboard::BitScanForward(pawn_single_pushes_no_promo);
        const auto from_square = to_square + PawnSinglePushOffset;

        move_list.Insert({from_square, to_square, PAWN, EMPTY_PIECE,
                          EMPTY_PIECE, false, false, false});

        bitboard::PopLastBit(pawn_single_pushes_no_promo);
    }
}

/// @brief Generates all the pawn capture moves with promotion for the given
/// board.
/// @tparam ToMove The color to generate pawn capture moves with promotion
/// for.
/// @param move_list The list of moves to add the pawn capture moves with
/// promotion to.
/// @param board The board to generate pawn capture moves with promotion on.
template <Color ToMove>
static void
PawnCapturesPromotion(MoveList &move_list, const Board &board) {
    static_assert(ToMove == WHITE or ToMove == BLACK, "Invalid color");

    constexpr auto Opponent = ToMove == WHITE ? BLACK : WHITE;
    constexpr auto MaskIndex = ToMove == WHITE ? 6 : 1;

    auto pawns_can_capture_with_promo =
        board.pieces(ToMove, PAWN) & utils::MASK_RANK[MaskIndex];

    while (pawns_can_capture_with_promo) {
        const auto from_square =
            bitboard::BitScanForward(pawns_can_capture_with_promo);

        auto pawn_captures_promo = attacks::PAWN_ATTACKS[ToMove][from_square] &
                                   board.occupancies(Opponent);

        while (pawn_captures_promo) {
            const auto to_square =
                bitboard::BitScanForward(pawn_captures_promo);

            move_list.Insert({from_square, to_square, PAWN,
                              board.piece(to_square).type, QUEEN, false, false,
                              false});

            move_list.Insert({from_square, to_square, PAWN,
                              board.piece(to_square).type, KNIGHT, false, false,
                              false});

            move_list.Insert({from_square, to_square, PAWN,
                              board.piece(to_square).type, ROOK, false, false,
                              false});

            move_list.Insert({from_square, to_square, PAWN,
                              board.piece(to_square).type, BISHOP, false, false,
                              false});

            bitboard::PopLastBit(pawn_captures_promo);
        }

        bitboard::PopLastBit(pawns_can_capture_with_promo);
    }
}

/// @brief Generates all the pawn capture moves without promotion for the given
/// board.
/// @tparam ToMove The color to generate pawn capture moves without promotion
/// for.
/// @param move_list The list of moves to add the pawn capture moves without
/// promotion to.
/// @param board The board to generate pawn capture moves without promotion on.
template <Color ToMove>
static void
PawnCapturesNoPromotion(MoveList &move_list, const Board &board) noexcept {
    static_assert(ToMove == WHITE or ToMove == BLACK, "Invalid color");

    constexpr auto Opponent = ToMove == WHITE ? BLACK : WHITE;
    constexpr auto MaskIndex = ToMove == WHITE ? 6 : 1;

    auto pawns_can_capture_no_promo =
        board.pieces(ToMove, PAWN) & utils::MASK_CLEAR_RANK[MaskIndex];

    while (pawns_can_capture_no_promo) {
        const auto from_square =
            bitboard::BitScanForward(pawns_can_capture_no_promo);

        auto pawn_captures_no_promo =
            attacks::PAWN_ATTACKS[ToMove][from_square] &
            board.occupancies(Opponent);

        while (pawn_captures_no_promo) {
            const auto to_square =
                bitboard::BitScanForward(pawn_captures_no_promo);

            move_list.Insert({from_square, to_square, PAWN,
                              board.piece(to_square).type, EMPTY_PIECE, false,
                              false, false});

            bitboard::PopLastBit(pawn_captures_no_promo);
        }
        bitboard::PopLastBit(pawns_can_capture_no_promo);
    }
}

/// @brief Generates all the pawn double push moves for
/// the given board.
/// @tparam ToMove The color to generate pawn double
/// push moves for.
/// @param move_list The list of moves to add the pawn
/// double push moves to.
/// @param board The board to generate pawn double push
/// moves on.
template <Color ToMove>
static void
PawnDoublePushes(MoveList &move_list, const Board &board) noexcept {
    static_assert(ToMove == WHITE or ToMove == BLACK, "Invalid color");

    constexpr auto PawnDoublePushOffset = ToMove == WHITE ? -16 : 16;

    auto pawn_double_pushes = attacks::PawnDoublePushes<ToMove>(
        board.pieces(ToMove, PAWN), ~board.occupancies(BOTH));

    while (pawn_double_pushes) {
        const auto to_square = bitboard::BitScanForward(pawn_double_pushes);
        const auto from_square = to_square + PawnDoublePushOffset;

        move_list.Insert({from_square, to_square, PAWN, EMPTY_PIECE,
                          EMPTY_PIECE, true, false, false});

        bitboard::PopLastBit(pawn_double_pushes);
    }
}

/// @brief Generates all the leaper moves for the
/// given board.
/// @tparam ToMove The color to generate leaper
/// moves for.
/// @tparam PType The type of piece to generate
/// leaper moves for.
/// @tparam GType The type of move generation to
/// perform.
/// @param move_list The list of moves to add the
/// leaper moves to.
/// @param board The board to generate leaper moves
/// on.
template <Color ToMove, PieceType PType, GenType GType>
static void
LeaperMoves(MoveList &move_list, const Board &board) noexcept {
    static_assert(ToMove == WHITE or ToMove == BLACK, "Invalid color");
    static_assert(PType == KNIGHT or PType == KING, "Invalid piece type");

    constexpr auto Opponent = ToMove == WHITE ? BLACK : WHITE;

    auto to_move_pieces = board.pieces(ToMove, PType);

    while (to_move_pieces) {
        const auto from_square = bitboard::BitScanForward(to_move_pieces);

        auto moves{bitboard::ZERO};
        if constexpr (GType == QUIETS) {
            if constexpr (PType == KNIGHT) {
                moves = attacks::KNIGHT_ATTACKS[from_square] &
                        ~board.occupancies(BOTH);
            } else {
                moves = attacks::KING_ATTACKS[from_square] &
                        ~board.occupancies(BOTH);
            }
        } else {
            if constexpr (PType == KNIGHT) {
                moves = attacks::KNIGHT_ATTACKS[from_square] &
                        ~board.occupancies(ToMove) &
                        board.occupancies(Opponent);
            } else {
                moves = attacks::KING_ATTACKS[from_square] &
                        ~board.occupancies(ToMove) &
                        board.occupancies(Opponent);
            }
        }

        while (moves) {
            const auto to_square = bitboard::BitScanForward(moves);

            if constexpr (GType == QUIETS) {
                move_list.Insert({from_square, to_square, PType, EMPTY_PIECE,
                                  EMPTY_PIECE, false, false, false});
            } else {
                move_list.Insert({from_square, to_square, PType,
                                  board.piece(to_square).type, EMPTY_PIECE,
                                  false, false, false});
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
template <Color ToMove, PieceType PType, GenType GType>
static void
SliderMoves(MoveList &move_list, const Board &board) {
    static_assert(ToMove == WHITE or ToMove == BLACK, "Invalid color");
    static_assert(PType == BISHOP or PType == ROOK or PType == QUEEN,
                  "Invalid piece type");

    constexpr auto Opponent = ToMove == WHITE ? BLACK : WHITE;

    constexpr auto ATTACKS_FUNC = (PType == BISHOP ? attacks::BishopAttacks
                                   : PType == ROOK ? attacks::RookAttacks
                                                   : attacks::QueenAttacks);

    auto to_move_pieces = board.pieces(ToMove, PType);
    while (to_move_pieces) {
        const auto from_square = bitboard::BitScanForward(to_move_pieces);

        auto moves{bitboard::ZERO};
        if constexpr (GType == QUIETS) {
            moves = ATTACKS_FUNC(from_square, board.occupancies(BOTH)) &
                    ~board.occupancies(ToMove) & ~board.occupancies(Opponent);
        } else {
            moves = ATTACKS_FUNC(from_square, board.occupancies(BOTH)) &
                    ~board.occupancies(ToMove) & board.occupancies(Opponent);
        }

        while (moves) {
            const auto to_square = bitboard::BitScanForward(moves);

            if constexpr (GType == QUIETS) {
                move_list.Insert({from_square, to_square, PType, EMPTY_PIECE,
                                  EMPTY_PIECE, false, false, false});
            } else {
                move_list.Insert({from_square, to_square, PType,
                                  board.piece(to_square).type, EMPTY_PIECE,
                                  false, false, false});
            }

            bitboard::PopLastBit(moves);
        }

        bitboard::PopLastBit(to_move_pieces);
    }
}

MoveList
PseudoLegal(const Board &board) noexcept {
    MoveList moves;

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

MoveList
PseudoLegalCaptures(const Board &board) noexcept {
    MoveList moves;

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

MoveList
Legal(Board &board) noexcept {
    MoveList moves;

    const auto state = board.GetStateBackup();
    for (const auto move : movegen::PseudoLegal(board)) {
        board.Make(move);

        const auto king_sq =
            bitboard::BitScanForward(board.pieces(board.inactive(), KING));

        if (!board.IsSquareAttacked(king_sq, board.active())) {
            moves.Insert(move);
        }

        board.Unmake(move, state);
    }

    return moves;
}

MoveList
LegalCaptures(Board &board) noexcept {
    MoveList captures;

    const auto state = board.GetStateBackup();
    for (const auto move : movegen::PseudoLegalCaptures(board)) {
        board.Make(move);

        const auto king_sq =
            bitboard::BitScanForward(board.pieces(board.inactive(), KING));

        if (!board.IsSquareAttacked(king_sq, board.active())) {
            captures.Insert(move);
        }

        board.Unmake(move, state);
    }

    return captures;
}

bool
HasLegal(Board &board) noexcept {
    const auto state = board.GetStateBackup();
    for (const auto move : PseudoLegal(board)) {
        board.Make(move);

        const auto king_sq =
            bitboard::BitScanForward(board.pieces(board.inactive(), KING));

        if (!board.IsSquareAttacked(king_sq, board.active())) {
            board.Unmake(move, state);
            return true;
        }

        board.Unmake(move, state);
    }

    return false;
}
}   // namespace codchess::movegen