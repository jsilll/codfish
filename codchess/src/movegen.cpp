#include <codchess/movegen.hpp>

#include "attacks.hpp"

namespace cod::chess::movegen {
/// @brief The type of move generation to perform.
enum class GenType {
    /// @brief Generate capture moves.
    Captures,
    /// @brief Generate quiet moves (non-captures).
    Quiets,
};

/// @brief Generates all the castling_availability moves for the given board.
/// @tparam ToMove The color to generate castling_availability moves for.
/// @param move_list The list of moves to add the castling_availability moves
/// to.
/// @param board The board to generate castling_availability moves on.
template <Color ToMove>
static void
CastlingMoves(MoveList &move_list, const Board &board) noexcept {
    static_assert(ToMove == Color::White or ToMove == Color::Black,
                  "Invalid color");

    constexpr auto Opponent =
        (ToMove == Color::White ? Color::Black : Color::White);

    constexpr auto CastleBSq =
        (ToMove == Color::White ? Square::B1 : Square::B8);
    constexpr auto CastleCSq =
        (ToMove == Color::White ? Square::C1 : Square::C8);
    constexpr auto CastleDSq =
        (ToMove == Color::White ? Square::D1 : Square::D8);
    constexpr auto CastleESq =
        (ToMove == Color::White ? Square::E1 : Square::E8);
    constexpr auto CastleFSq =
        (ToMove == Color::White ? Square::F1 : Square::F8);
    constexpr auto CastleGSq =
        (ToMove == Color::White ? Square::G1 : Square::G8);

    constexpr auto CastleKingMask =
        (ToMove == Color::White ? Castle::WhiteKing
                                : Castle::BlackKing);
    constexpr auto CastleQueenMask =
        (ToMove == Color::White ? Castle::WhiteQueen
                                : Castle::BlackQueen);

    if (!board.IsSquareAttacked(CastleESq, Opponent)) {
        if ((static_cast<std::uint8_t>(board.castling_availability()) &
             static_cast<std::uint8_t>(CastleKingMask)) &&
            !bitboard::GetBit(board.occupancies(Color::Both), CastleFSq) &&
            !bitboard::GetBit(board.occupancies(Color::Both), CastleGSq)) {
            if (!board.IsSquareAttacked(CastleFSq, Opponent) &&
                !board.IsSquareAttacked(CastleGSq, Opponent)) {

                move_list.Insert({CastleESq, CastleGSq, Piece::King,
                                  Piece::Empty, Piece::Empty, false,
                                  false, true});
            }
        }
        if ((static_cast<std::uint8_t>(board.castling_availability()) &
             static_cast<std::uint8_t>(CastleQueenMask)) &&
            !bitboard::GetBit(board.occupancies(Color::Both), CastleDSq) &&
            !bitboard::GetBit(board.occupancies(Color::Both), CastleCSq) &&
            !bitboard::GetBit(board.occupancies(Color::Both), CastleBSq)) {
            if (!board.IsSquareAttacked(CastleDSq, Opponent) &&
                !board.IsSquareAttacked(CastleCSq, Opponent)) {

                move_list.Insert({CastleESq, CastleCSq, Piece::King,
                                  Piece::Empty, Piece::Empty, false,
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
    static_assert(ToMove == Color::White or ToMove == Color::Black,
                  "Invalid color");

    constexpr auto Opponent =
        ToMove == Color::White ? Color::Black : Color::White;

    if (board.en_passant_square() != Square::Empty) {
        auto pawns_can_en_passant =
            attacks::kPawnAttacks[static_cast<std::size_t>(Opponent)]
                                 [static_cast<std::size_t>(
                                     board.en_passant_square())] &
            board.pieces(ToMove, Piece::Pawn);

        while (pawns_can_en_passant) {
            const auto from_square =
                bitboard::BitScanForward(pawns_can_en_passant);

            move_list.Insert({from_square, board.en_passant_square(),
                              Piece::Pawn,
                              board.piece(board.en_passant_square()).type,
                              Piece::Empty, false, true, false});

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
    static_assert(ToMove == Color::White or ToMove == Color::Black,
                  "Invalid color");

    constexpr auto PawnSinglePushOffset = ToMove == Color::White ? -8 : 8;

    auto pawn_single_pushes_promo =
        attacks::PawnSinglePushes<ToMove>(board.pieces(ToMove, Piece::Pawn),
                                          ~board.occupancies(Color::Both)) &
        (utils::kMaskRank[0] | utils::kMaskRank[7]);

    while (pawn_single_pushes_promo) {
        const auto to_square =
            bitboard::BitScanForward(pawn_single_pushes_promo);
        const auto from_square = static_cast<Square>(
            static_cast<std::uint8_t>(to_square) + PawnSinglePushOffset);

        move_list.Insert({from_square, to_square, Piece::Pawn,
                          Piece::Empty, Piece::Queen, false, false,
                          false});

        move_list.Insert({from_square, to_square, Piece::Pawn,
                          Piece::Empty, Piece::Knight, false, false,
                          false});

        move_list.Insert({from_square, to_square, Piece::Pawn,
                          Piece::Empty, Piece::Rook, false, false,
                          false});

        move_list.Insert({from_square, to_square, Piece::Pawn,
                          Piece::Empty, Piece::Bishop, false, false,
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
    static_assert(ToMove == Color::White or ToMove == Color::Black,
                  "Invalid color");

    constexpr auto PawnSinglePushOffset = ToMove == Color::White ? -8 : 8;

    auto pawn_single_pushes_no_promo =
        attacks::PawnSinglePushes<ToMove>(board.pieces(ToMove, Piece::Pawn),
                                          ~board.occupancies(Color::Both)) &
        utils::kMaskNotRank[0] & utils::kMaskNotRank[7];

    while (pawn_single_pushes_no_promo) {
        const auto to_square =
            bitboard::BitScanForward(pawn_single_pushes_no_promo);
        const auto from_square = static_cast<Square>(
            static_cast<std::uint8_t>(to_square) + PawnSinglePushOffset);

        move_list.Insert({from_square, to_square, Piece::Pawn,
                          Piece::Empty, Piece::Empty, false, false,
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
    static_assert(ToMove == Color::White or ToMove == Color::Black,
                  "Invalid color");

    constexpr auto Opponent =
        ToMove == Color::White ? Color::Black : Color::White;
    constexpr auto MaskIndex = ToMove == Color::White ? 6 : 1;

    auto pawns_can_capture_with_promo =
        board.pieces(ToMove, Piece::Pawn) & utils::kMaskRank[MaskIndex];

    while (pawns_can_capture_with_promo) {
        const auto from_square =
            bitboard::BitScanForward(pawns_can_capture_with_promo);

        auto pawn_captures_promo =
            attacks::kPawnAttacks[static_cast<std::size_t>(ToMove)]
                                 [static_cast<std::size_t>(from_square)] &
            board.occupancies(Opponent);

        while (pawn_captures_promo) {
            const auto to_square =
                bitboard::BitScanForward(pawn_captures_promo);

            move_list.Insert({from_square, to_square, Piece::Pawn,
                              board.piece(to_square).type, Piece::Queen, false,
                              false, false});

            move_list.Insert({from_square, to_square, Piece::Pawn,
                              board.piece(to_square).type, Piece::Knight, false,
                              false, false});

            move_list.Insert({from_square, to_square, Piece::Pawn,
                              board.piece(to_square).type, Piece::Rook, false,
                              false, false});

            move_list.Insert({from_square, to_square, Piece::Pawn,
                              board.piece(to_square).type, Piece::Bishop, false,
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
    static_assert(ToMove == Color::White or ToMove == Color::Black,
                  "Invalid color");

    constexpr auto Opponent =
        ToMove == Color::White ? Color::Black : Color::White;
    constexpr auto MaskIndex = ToMove == Color::White ? 6 : 1;

    auto pawns_can_capture_no_promo =
        board.pieces(ToMove, Piece::Pawn) & utils::kMaskNotRank[MaskIndex];

    while (pawns_can_capture_no_promo) {
        const auto from_square =
            bitboard::BitScanForward(pawns_can_capture_no_promo);

        auto pawn_captures_no_promo =
            attacks::kPawnAttacks[static_cast<std::size_t>(ToMove)]
                                 [static_cast<std::size_t>(from_square)] &
            board.occupancies(Opponent);

        while (pawn_captures_no_promo) {
            const auto to_square =
                bitboard::BitScanForward(pawn_captures_no_promo);

            move_list.Insert({from_square, to_square, Piece::Pawn,
                              board.piece(to_square).type, Piece::Empty,
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
    static_assert(ToMove == Color::White or ToMove == Color::Black,
                  "Invalid color");

    constexpr auto PawnDoublePushOffset = ToMove == Color::White ? -16 : 16;

    auto pawn_double_pushes = attacks::PawnDoublePushes<ToMove>(
        board.pieces(ToMove, Piece::Pawn), ~board.occupancies(Color::Both));

    while (pawn_double_pushes) {
        const auto to_square = bitboard::BitScanForward(pawn_double_pushes);
        const auto from_square = static_cast<Square>(
            static_cast<std::uint8_t>(to_square) + PawnDoublePushOffset);

        move_list.Insert({from_square, to_square, Piece::Pawn,
                          Piece::Empty, Piece::Empty, true, false,
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
    static_assert(ToMove == Color::White or ToMove == Color::Black,
                  "Invalid color");
    static_assert(PType == Piece::Knight or PType == Piece::King,
                  "Invalid piece type");

    constexpr auto Opponent =
        ToMove == Color::White ? Color::Black : Color::White;

    auto to_move_pieces = board.pieces(ToMove, PType);

    while (to_move_pieces) {
        const auto from_square = bitboard::BitScanForward(to_move_pieces);

        auto moves{bitboard::kZero};
        if constexpr (GType == GenType::Quiets) {
            if constexpr (PType == Piece::Knight) {
                moves = attacks::kKnightAttacks[static_cast<std::size_t>(
                            from_square)] &
                        ~board.occupancies(Color::Both);
            } else {
                moves = attacks::kKingAttacks[static_cast<std::size_t>(
                            from_square)] &
                        ~board.occupancies(Color::Both);
            }
        } else {
            if constexpr (PType == Piece::Knight) {
                moves = attacks::kKnightAttacks[static_cast<std::size_t>(
                            from_square)] &
                        ~board.occupancies(ToMove) &
                        board.occupancies(Opponent);
            } else {
                moves = attacks::kKingAttacks[static_cast<std::size_t>(
                            from_square)] &
                        ~board.occupancies(ToMove) &
                        board.occupancies(Opponent);
            }
        }

        while (moves) {
            const auto to_square = bitboard::BitScanForward(moves);

            if constexpr (GType == GenType::Quiets) {
                move_list.Insert({from_square, to_square, PType,
                                  Piece::Empty, Piece::Empty, false,
                                  false, false});
            } else {
                move_list.Insert({from_square, to_square, PType,
                                  board.piece(to_square).type,
                                  Piece::Empty, false, false, false});
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
    static_assert(ToMove == Color::White or ToMove == Color::Black,
                  "Invalid color");
    static_assert(PType == Piece::Bishop or PType == Piece::Rook or
                      PType == Piece::Queen,
                  "Invalid piece type");

    constexpr auto Opponent =
        ToMove == Color::White ? Color::Black : Color::White;

    constexpr auto ATTACKS_FUNC =
        (PType == Piece::Bishop ? attacks::BishopAttacks
         : PType == Piece::Rook ? attacks::RookAttacks
                                : attacks::QueenAttacks);

    auto to_move_pieces = board.pieces(ToMove, PType);
    while (to_move_pieces) {
        const auto from_square = bitboard::BitScanForward(to_move_pieces);

        auto moves{bitboard::kZero};
        if constexpr (GType == GenType::Quiets) {
            moves = ATTACKS_FUNC(from_square, board.occupancies(Color::Both)) &
                    ~board.occupancies(ToMove) & ~board.occupancies(Opponent);
        } else {
            moves = ATTACKS_FUNC(from_square, board.occupancies(Color::Both)) &
                    ~board.occupancies(ToMove) & board.occupancies(Opponent);
        }

        while (moves) {
            const auto to_square = bitboard::BitScanForward(moves);

            if constexpr (GType == GenType::Quiets) {
                move_list.Insert({from_square, to_square, PType,
                                  Piece::Empty, Piece::Empty, false,
                                  false, false});
            } else {
                move_list.Insert({from_square, to_square, PType,
                                  board.piece(to_square).type,
                                  Piece::Empty, false, false, false});
            }

            bitboard::PopLastBit(moves);
        }

        bitboard::PopLastBit(to_move_pieces);
    }
}

MoveList
PseudoLegal(const Board &board) noexcept {
    MoveList moves;

    if (board.active() == Color::White) {
        PawnCapturesPromotion<Color::White>(moves, board);
        PawnCapturesNoPromotion<Color::White>(moves, board);
        EnPassantCaptures<Color::White>(moves, board);
        PawnSinglePushPromotions<Color::White>(moves, board);
        PawnDoublePushes<Color::White>(moves, board);
        PawnSinglePushesNoPromotion<Color::White>(moves, board);

        LeaperMoves<Color::White, Piece::Knight, GenType::Captures>(moves,
                                                                    board);
        LeaperMoves<Color::White, Piece::King, GenType::Captures>(moves, board);
        SliderMoves<Color::White, Piece::Bishop, GenType::Captures>(moves,
                                                                    board);
        SliderMoves<Color::White, Piece::Rook, GenType::Captures>(moves, board);
        SliderMoves<Color::White, Piece::Queen, GenType::Captures>(moves,
                                                                   board);

        LeaperMoves<Color::White, Piece::Knight, GenType::Quiets>(moves, board);
        LeaperMoves<Color::White, Piece::King, GenType::Quiets>(moves, board);
        SliderMoves<Color::White, Piece::Bishop, GenType::Quiets>(moves, board);
        SliderMoves<Color::White, Piece::Rook, GenType::Quiets>(moves, board);
        SliderMoves<Color::White, Piece::Queen, GenType::Quiets>(moves, board);

        CastlingMoves<Color::White>(moves, board);
    } else {
        PawnCapturesPromotion<Color::Black>(moves, board);
        PawnCapturesNoPromotion<Color::Black>(moves, board);
        EnPassantCaptures<Color::Black>(moves, board);
        PawnSinglePushPromotions<Color::Black>(moves, board);
        PawnDoublePushes<Color::Black>(moves, board);
        PawnSinglePushesNoPromotion<Color::Black>(moves, board);

        LeaperMoves<Color::Black, Piece::Knight, GenType::Captures>(moves,
                                                                    board);
        LeaperMoves<Color::Black, Piece::King, GenType::Captures>(moves, board);
        SliderMoves<Color::Black, Piece::Bishop, GenType::Captures>(moves,
                                                                    board);
        SliderMoves<Color::Black, Piece::Rook, GenType::Captures>(moves, board);
        SliderMoves<Color::Black, Piece::Queen, GenType::Captures>(moves,
                                                                   board);

        LeaperMoves<Color::Black, Piece::Knight, GenType::Quiets>(moves, board);
        LeaperMoves<Color::Black, Piece::King, GenType::Quiets>(moves, board);
        SliderMoves<Color::Black, Piece::Bishop, GenType::Quiets>(moves, board);
        SliderMoves<Color::Black, Piece::Rook, GenType::Quiets>(moves, board);
        SliderMoves<Color::Black, Piece::Queen, GenType::Quiets>(moves, board);

        CastlingMoves<Color::Black>(moves, board);
    }

    return moves;
}

MoveList
PseudoLegalCaptures(const Board &board) noexcept {
    MoveList moves;

    if (board.active() == Color::White) {
        PawnCapturesPromotion<Color::White>(moves, board);
        PawnCapturesNoPromotion<Color::White>(moves, board);
        EnPassantCaptures<Color::White>(moves, board);

        LeaperMoves<Color::White, Piece::Knight, GenType::Captures>(moves,
                                                                    board);
        LeaperMoves<Color::White, Piece::King, GenType::Captures>(moves, board);

        SliderMoves<Color::White, Piece::Bishop, GenType::Captures>(moves,
                                                                    board);
        SliderMoves<Color::White, Piece::Rook, GenType::Captures>(moves, board);
        SliderMoves<Color::White, Piece::Queen, GenType::Captures>(moves,
                                                                   board);
    } else {
        PawnCapturesPromotion<Color::Black>(moves, board);
        PawnCapturesNoPromotion<Color::Black>(moves, board);
        EnPassantCaptures<Color::Black>(moves, board);

        LeaperMoves<Color::Black, Piece::Knight, GenType::Captures>(moves,
                                                                    board);
        LeaperMoves<Color::Black, Piece::King, GenType::Captures>(moves, board);

        SliderMoves<Color::Black, Piece::Bishop, GenType::Captures>(moves,
                                                                    board);
        SliderMoves<Color::Black, Piece::Rook, GenType::Captures>(moves, board);
        SliderMoves<Color::Black, Piece::Queen, GenType::Captures>(moves,
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
            board.pieces(board.inactive(), Piece::King));

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
            board.pieces(board.inactive(), Piece::King));

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
            board.pieces(board.inactive(), Piece::King));

        if (!board.IsSquareAttacked(king_sq, board.active())) {
            board.Unmake(move, state);
            return true;
        }

        board.Unmake(move, state);
    }

    return false;
}
}   // namespace cod::chess::movegen