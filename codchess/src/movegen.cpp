#include <codchess/movegen.hpp>

#include "attacks.hpp"

namespace cod::chess::movegen {
/// @brief The type of move generation to perform.
enum class GenType {
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
    static_assert(ToMove == Color::WHITE or ToMove == Color::BLACK,
                  "Invalid color");

    constexpr auto Opponent =
        (ToMove == Color::WHITE ? Color::BLACK : Color::WHITE);

    constexpr auto CastleBSq =
        (ToMove == Color::WHITE ? Square::B1 : Square::B8);
    constexpr auto CastleCSq =
        (ToMove == Color::WHITE ? Square::C1 : Square::C8);
    constexpr auto CastleDSq =
        (ToMove == Color::WHITE ? Square::D1 : Square::D8);
    constexpr auto CastleESq =
        (ToMove == Color::WHITE ? Square::E1 : Square::E8);
    constexpr auto CastleFSq =
        (ToMove == Color::WHITE ? Square::F1 : Square::F8);
    constexpr auto CastleGSq =
        (ToMove == Color::WHITE ? Square::G1 : Square::G8);

    constexpr auto CastleKingMask =
        (ToMove == Color::WHITE ? CastlingAvailability::WHITE_KING
                                : CastlingAvailability::BLACK_KING);
    constexpr auto CastleQueenMask =
        (ToMove == Color::WHITE ? CastlingAvailability::WHITE_QUEEN
                                : CastlingAvailability::BLACK_QUEEN);

    if (!board.IsSquareAttacked(CastleESq, Opponent)) {
        if ((static_cast<std::uint8_t>(board.castling_availability()) &
             static_cast<std::uint8_t>(CastleKingMask)) &&
            !bitboard::GetBit(board.occupancies(Color::BOTH), CastleFSq) &&
            !bitboard::GetBit(board.occupancies(Color::BOTH), CastleGSq)) {
            if (!board.IsSquareAttacked(CastleFSq, Opponent) &&
                !board.IsSquareAttacked(CastleGSq, Opponent)) {

                move_list.Insert({CastleESq, CastleGSq, Piece::KING,
                                  Piece::EMPTY_PIECE, Piece::EMPTY_PIECE, false,
                                  false, true});
            }
        }
        if ((static_cast<std::uint8_t>(board.castling_availability()) &
             static_cast<std::uint8_t>(CastleQueenMask)) &&
            !bitboard::GetBit(board.occupancies(Color::BOTH), CastleDSq) &&
            !bitboard::GetBit(board.occupancies(Color::BOTH), CastleCSq) &&
            !bitboard::GetBit(board.occupancies(Color::BOTH), CastleBSq)) {
            if (!board.IsSquareAttacked(CastleDSq, Opponent) &&
                !board.IsSquareAttacked(CastleCSq, Opponent)) {

                move_list.Insert({CastleESq, CastleCSq, Piece::KING,
                                  Piece::EMPTY_PIECE, Piece::EMPTY_PIECE, false,
                                  false, true});
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
    static_assert(ToMove == Color::WHITE or ToMove == Color::BLACK,
                  "Invalid color");

    constexpr auto Opponent =
        ToMove == Color::WHITE ? Color::BLACK : Color::WHITE;

    if (board.en_passant_square() != Square::EMPTY_SQUARE) {
        auto pawns_can_en_passant =
            attacks::PAWN_ATTACKS[static_cast<std::size_t>(Opponent)]
                                 [static_cast<std::size_t>(
                                     board.en_passant_square())] &
            board.pieces(ToMove, Piece::PAWN);

        while (pawns_can_en_passant) {
            const auto from_square =
                bitboard::BitScanForward(pawns_can_en_passant);

            move_list.Insert({from_square, board.en_passant_square(),
                              Piece::PAWN,
                              board.piece(board.en_passant_square()).type,
                              Piece::EMPTY_PIECE, false, true, false});

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
    static_assert(ToMove == Color::WHITE or ToMove == Color::BLACK,
                  "Invalid color");

    constexpr auto PawnSinglePushOffset = ToMove == Color::WHITE ? -8 : 8;

    auto pawn_single_pushes_promo =
        attacks::PawnSinglePushes<ToMove>(board.pieces(ToMove, Piece::PAWN),
                                          ~board.occupancies(Color::BOTH)) &
        (utils::MASK_RANK[0] | utils::MASK_RANK[7]);

    while (pawn_single_pushes_promo) {
        const auto to_square =
            bitboard::BitScanForward(pawn_single_pushes_promo);
        const auto from_square = static_cast<Square>(
            static_cast<std::uint8_t>(to_square) + PawnSinglePushOffset);

        move_list.Insert({from_square, to_square, Piece::PAWN,
                          Piece::EMPTY_PIECE, Piece::QUEEN, false, false,
                          false});

        move_list.Insert({from_square, to_square, Piece::PAWN,
                          Piece::EMPTY_PIECE, Piece::KNIGHT, false, false,
                          false});

        move_list.Insert({from_square, to_square, Piece::PAWN,
                          Piece::EMPTY_PIECE, Piece::ROOK, false, false,
                          false});

        move_list.Insert({from_square, to_square, Piece::PAWN,
                          Piece::EMPTY_PIECE, Piece::BISHOP, false, false,
                          false});

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
    static_assert(ToMove == Color::WHITE or ToMove == Color::BLACK,
                  "Invalid color");

    constexpr auto PawnSinglePushOffset = ToMove == Color::WHITE ? -8 : 8;

    auto pawn_single_pushes_no_promo =
        attacks::PawnSinglePushes<ToMove>(board.pieces(ToMove, Piece::PAWN),
                                          ~board.occupancies(Color::BOTH)) &
        utils::MASK_NOT_RANK[0] & utils::MASK_NOT_RANK[7];

    while (pawn_single_pushes_no_promo) {
        const auto to_square =
            bitboard::BitScanForward(pawn_single_pushes_no_promo);
        const auto from_square = static_cast<Square>(
            static_cast<std::uint8_t>(to_square) + PawnSinglePushOffset);

        move_list.Insert({from_square, to_square, Piece::PAWN,
                          Piece::EMPTY_PIECE, Piece::EMPTY_PIECE, false, false,
                          false});

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
    static_assert(ToMove == Color::WHITE or ToMove == Color::BLACK,
                  "Invalid color");

    constexpr auto Opponent =
        ToMove == Color::WHITE ? Color::BLACK : Color::WHITE;
    constexpr auto MaskIndex = ToMove == Color::WHITE ? 6 : 1;

    auto pawns_can_capture_with_promo =
        board.pieces(ToMove, Piece::PAWN) & utils::MASK_RANK[MaskIndex];

    while (pawns_can_capture_with_promo) {
        const auto from_square =
            bitboard::BitScanForward(pawns_can_capture_with_promo);

        auto pawn_captures_promo =
            attacks::PAWN_ATTACKS[static_cast<std::size_t>(ToMove)]
                                 [static_cast<std::size_t>(from_square)] &
            board.occupancies(Opponent);

        while (pawn_captures_promo) {
            const auto to_square =
                bitboard::BitScanForward(pawn_captures_promo);

            move_list.Insert({from_square, to_square, Piece::PAWN,
                              board.piece(to_square).type, Piece::QUEEN, false,
                              false, false});

            move_list.Insert({from_square, to_square, Piece::PAWN,
                              board.piece(to_square).type, Piece::KNIGHT, false,
                              false, false});

            move_list.Insert({from_square, to_square, Piece::PAWN,
                              board.piece(to_square).type, Piece::ROOK, false,
                              false, false});

            move_list.Insert({from_square, to_square, Piece::PAWN,
                              board.piece(to_square).type, Piece::BISHOP, false,
                              false, false});

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
    static_assert(ToMove == Color::WHITE or ToMove == Color::BLACK,
                  "Invalid color");

    constexpr auto Opponent =
        ToMove == Color::WHITE ? Color::BLACK : Color::WHITE;
    constexpr auto MaskIndex = ToMove == Color::WHITE ? 6 : 1;

    auto pawns_can_capture_no_promo =
        board.pieces(ToMove, Piece::PAWN) & utils::MASK_NOT_RANK[MaskIndex];

    while (pawns_can_capture_no_promo) {
        const auto from_square =
            bitboard::BitScanForward(pawns_can_capture_no_promo);

        auto pawn_captures_no_promo =
            attacks::PAWN_ATTACKS[static_cast<std::size_t>(ToMove)]
                                 [static_cast<std::size_t>(from_square)] &
            board.occupancies(Opponent);

        while (pawn_captures_no_promo) {
            const auto to_square =
                bitboard::BitScanForward(pawn_captures_no_promo);

            move_list.Insert({from_square, to_square, Piece::PAWN,
                              board.piece(to_square).type, Piece::EMPTY_PIECE,
                              false, false, false});

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
    static_assert(ToMove == Color::WHITE or ToMove == Color::BLACK,
                  "Invalid color");

    constexpr auto PawnDoublePushOffset = ToMove == Color::WHITE ? -16 : 16;

    auto pawn_double_pushes = attacks::PawnDoublePushes<ToMove>(
        board.pieces(ToMove, Piece::PAWN), ~board.occupancies(Color::BOTH));

    while (pawn_double_pushes) {
        const auto to_square = bitboard::BitScanForward(pawn_double_pushes);
        const auto from_square = static_cast<Square>(
            static_cast<std::uint8_t>(to_square) + PawnDoublePushOffset);

        move_list.Insert({from_square, to_square, Piece::PAWN,
                          Piece::EMPTY_PIECE, Piece::EMPTY_PIECE, true, false,
                          false});

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
template <Color ToMove, Piece PType, GenType GType>
static void
LeaperMoves(MoveList &move_list, const Board &board) noexcept {
    static_assert(ToMove == Color::WHITE or ToMove == Color::BLACK,
                  "Invalid color");
    static_assert(PType == Piece::KNIGHT or PType == Piece::KING,
                  "Invalid piece type");

    constexpr auto Opponent =
        ToMove == Color::WHITE ? Color::BLACK : Color::WHITE;

    auto to_move_pieces = board.pieces(ToMove, PType);

    while (to_move_pieces) {
        const auto from_square = bitboard::BitScanForward(to_move_pieces);

        auto moves{bitboard::ZERO};
        if constexpr (GType == GenType::QUIETS) {
            if constexpr (PType == Piece::KNIGHT) {
                moves = attacks::KNIGHT_ATTACKS[static_cast<std::size_t>(
                            from_square)] &
                        ~board.occupancies(Color::BOTH);
            } else {
                moves = attacks::KING_ATTACKS[static_cast<std::size_t>(
                            from_square)] &
                        ~board.occupancies(Color::BOTH);
            }
        } else {
            if constexpr (PType == Piece::KNIGHT) {
                moves = attacks::KNIGHT_ATTACKS[static_cast<std::size_t>(
                            from_square)] &
                        ~board.occupancies(ToMove) &
                        board.occupancies(Opponent);
            } else {
                moves = attacks::KING_ATTACKS[static_cast<std::size_t>(
                            from_square)] &
                        ~board.occupancies(ToMove) &
                        board.occupancies(Opponent);
            }
        }

        while (moves) {
            const auto to_square = bitboard::BitScanForward(moves);

            if constexpr (GType == GenType::QUIETS) {
                move_list.Insert({from_square, to_square, PType,
                                  Piece::EMPTY_PIECE, Piece::EMPTY_PIECE, false,
                                  false, false});
            } else {
                move_list.Insert({from_square, to_square, PType,
                                  board.piece(to_square).type,
                                  Piece::EMPTY_PIECE, false, false, false});
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
template <Color ToMove, Piece PType, GenType GType>
static void
SliderMoves(MoveList &move_list, const Board &board) {
    static_assert(ToMove == Color::WHITE or ToMove == Color::BLACK,
                  "Invalid color");
    static_assert(PType == Piece::BISHOP or PType == Piece::ROOK or
                      PType == Piece::QUEEN,
                  "Invalid piece type");

    constexpr auto Opponent =
        ToMove == Color::WHITE ? Color::BLACK : Color::WHITE;

    constexpr auto ATTACKS_FUNC =
        (PType == Piece::BISHOP ? attacks::BishopAttacks
         : PType == Piece::ROOK ? attacks::RookAttacks
                                : attacks::QueenAttacks);

    auto to_move_pieces = board.pieces(ToMove, PType);
    while (to_move_pieces) {
        const auto from_square = bitboard::BitScanForward(to_move_pieces);

        auto moves{bitboard::ZERO};
        if constexpr (GType == GenType::QUIETS) {
            moves = ATTACKS_FUNC(from_square, board.occupancies(Color::BOTH)) &
                    ~board.occupancies(ToMove) & ~board.occupancies(Opponent);
        } else {
            moves = ATTACKS_FUNC(from_square, board.occupancies(Color::BOTH)) &
                    ~board.occupancies(ToMove) & board.occupancies(Opponent);
        }

        while (moves) {
            const auto to_square = bitboard::BitScanForward(moves);

            if constexpr (GType == GenType::QUIETS) {
                move_list.Insert({from_square, to_square, PType,
                                  Piece::EMPTY_PIECE, Piece::EMPTY_PIECE, false,
                                  false, false});
            } else {
                move_list.Insert({from_square, to_square, PType,
                                  board.piece(to_square).type,
                                  Piece::EMPTY_PIECE, false, false, false});
            }

            bitboard::PopLastBit(moves);
        }

        bitboard::PopLastBit(to_move_pieces);
    }
}

MoveList
PseudoLegal(const Board &board) noexcept {
    MoveList moves;

    if (board.active() == Color::WHITE) {
        PawnCapturesPromotion<Color::WHITE>(moves, board);
        PawnCapturesNoPromotion<Color::WHITE>(moves, board);
        EnPassantCaptures<Color::WHITE>(moves, board);
        PawnSinglePushPromotions<Color::WHITE>(moves, board);
        PawnDoublePushes<Color::WHITE>(moves, board);
        PawnSinglePushesNoPromotion<Color::WHITE>(moves, board);

        LeaperMoves<Color::WHITE, Piece::KNIGHT, GenType::CAPTURES>(moves,
                                                                    board);
        LeaperMoves<Color::WHITE, Piece::KING, GenType::CAPTURES>(moves, board);
        SliderMoves<Color::WHITE, Piece::BISHOP, GenType::CAPTURES>(moves,
                                                                    board);
        SliderMoves<Color::WHITE, Piece::ROOK, GenType::CAPTURES>(moves, board);
        SliderMoves<Color::WHITE, Piece::QUEEN, GenType::CAPTURES>(moves,
                                                                   board);

        LeaperMoves<Color::WHITE, Piece::KNIGHT, GenType::QUIETS>(moves, board);
        LeaperMoves<Color::WHITE, Piece::KING, GenType::QUIETS>(moves, board);
        SliderMoves<Color::WHITE, Piece::BISHOP, GenType::QUIETS>(moves, board);
        SliderMoves<Color::WHITE, Piece::ROOK, GenType::QUIETS>(moves, board);
        SliderMoves<Color::WHITE, Piece::QUEEN, GenType::QUIETS>(moves, board);

        CastlingMoves<Color::WHITE>(moves, board);
    } else {
        PawnCapturesPromotion<Color::BLACK>(moves, board);
        PawnCapturesNoPromotion<Color::BLACK>(moves, board);
        EnPassantCaptures<Color::BLACK>(moves, board);
        PawnSinglePushPromotions<Color::BLACK>(moves, board);
        PawnDoublePushes<Color::BLACK>(moves, board);
        PawnSinglePushesNoPromotion<Color::BLACK>(moves, board);

        LeaperMoves<Color::BLACK, Piece::KNIGHT, GenType::CAPTURES>(moves,
                                                                    board);
        LeaperMoves<Color::BLACK, Piece::KING, GenType::CAPTURES>(moves, board);
        SliderMoves<Color::BLACK, Piece::BISHOP, GenType::CAPTURES>(moves,
                                                                    board);
        SliderMoves<Color::BLACK, Piece::ROOK, GenType::CAPTURES>(moves, board);
        SliderMoves<Color::BLACK, Piece::QUEEN, GenType::CAPTURES>(moves,
                                                                   board);

        LeaperMoves<Color::BLACK, Piece::KNIGHT, GenType::QUIETS>(moves, board);
        LeaperMoves<Color::BLACK, Piece::KING, GenType::QUIETS>(moves, board);
        SliderMoves<Color::BLACK, Piece::BISHOP, GenType::QUIETS>(moves, board);
        SliderMoves<Color::BLACK, Piece::ROOK, GenType::QUIETS>(moves, board);
        SliderMoves<Color::BLACK, Piece::QUEEN, GenType::QUIETS>(moves, board);

        CastlingMoves<Color::BLACK>(moves, board);
    }

    return moves;
}

MoveList
PseudoLegalCaptures(const Board &board) noexcept {
    MoveList moves;

    if (board.active() == Color::WHITE) {
        PawnCapturesPromotion<Color::WHITE>(moves, board);
        PawnCapturesNoPromotion<Color::WHITE>(moves, board);
        EnPassantCaptures<Color::WHITE>(moves, board);

        LeaperMoves<Color::WHITE, Piece::KNIGHT, GenType::CAPTURES>(moves,
                                                                    board);
        LeaperMoves<Color::WHITE, Piece::KING, GenType::CAPTURES>(moves, board);

        SliderMoves<Color::WHITE, Piece::BISHOP, GenType::CAPTURES>(moves,
                                                                    board);
        SliderMoves<Color::WHITE, Piece::ROOK, GenType::CAPTURES>(moves, board);
        SliderMoves<Color::WHITE, Piece::QUEEN, GenType::CAPTURES>(moves,
                                                                   board);
    } else {
        PawnCapturesPromotion<Color::BLACK>(moves, board);
        PawnCapturesNoPromotion<Color::BLACK>(moves, board);
        EnPassantCaptures<Color::BLACK>(moves, board);

        LeaperMoves<Color::BLACK, Piece::KNIGHT, GenType::CAPTURES>(moves,
                                                                    board);
        LeaperMoves<Color::BLACK, Piece::KING, GenType::CAPTURES>(moves, board);

        SliderMoves<Color::BLACK, Piece::BISHOP, GenType::CAPTURES>(moves,
                                                                    board);
        SliderMoves<Color::BLACK, Piece::ROOK, GenType::CAPTURES>(moves, board);
        SliderMoves<Color::BLACK, Piece::QUEEN, GenType::CAPTURES>(moves,
                                                                   board);
    }

    return moves;
}

MoveList
Legal(Board &board) noexcept {
    MoveList moves;

    const auto state = board.GetStateBackup();
    for (const auto move : movegen::PseudoLegal(board)) {
        board.Make(move);

        const auto king_sq = bitboard::BitScanForward(
            board.pieces(board.inactive(), Piece::KING));

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

        const auto king_sq = bitboard::BitScanForward(
            board.pieces(board.inactive(), Piece::KING));

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

        const auto king_sq = bitboard::BitScanForward(
            board.pieces(board.inactive(), Piece::KING));

        if (!board.IsSquareAttacked(king_sq, board.active())) {
            board.Unmake(move, state);
            return true;
        }

        board.Unmake(move, state);
    }

    return false;
}
}   // namespace cod::chess::movegen