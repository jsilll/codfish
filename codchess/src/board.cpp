#include <codchess/board.hpp>

#include <codchess/movegen.hpp>

#include "attacks.hpp"
#include "zobrist.hpp"

namespace cod::chess {
[[maybe_unused]] std::string
Board::ToFen() const noexcept {
    int empty_squares{0};
    std::string piece_placements;
    for (int rank = static_cast<int>(Rank::R8);
         rank >= static_cast<int>(Rank::R1); --rank) {
        for (int file = static_cast<int>(File::FA);
             file < static_cast<int>(File::Total); ++file) {
            Square sq = utils::GetSquare(static_cast<Rank>(rank),
                                         static_cast<File>(file));
            if (file == 0) {
                if (empty_squares) {
                    piece_placements += std::to_string(empty_squares);
                    empty_squares = 0;
                }
                piece_placements += '/';
            }
            switch (_piece[static_cast<std::size_t>(sq)].type) {
            case Piece::Empty:
                empty_squares++;
                break;
            default:
                if (empty_squares) {
                    piece_placements += std::to_string(empty_squares);
                    empty_squares = 0;
                }
                piece_placements += utils::PieceToString(
                    _piece[static_cast<std::size_t>(sq)].type,
                    _piece[static_cast<std::size_t>(sq)].color);
                break;
            }
        }
        if (empty_squares) {
            piece_placements += std::to_string(empty_squares);
            empty_squares = 0;
        }
    }

    const auto active_color = _active == Color::White ? "w" : "b";

    char castling_rights_buf[5];
    snprintf(castling_rights_buf, 5, "%s%s%s%s",
             (static_cast<std::uint8_t>(_castling_availability) &
              static_cast<std::uint8_t>(Castle::WhiteKing))
                 ? "K"
                 : "",
             (static_cast<std::uint8_t>(_castling_availability) &
              static_cast<std::uint8_t>(Castle::WhiteQueen))
                 ? "Q"
                 : "",
             (static_cast<std::uint8_t>(_castling_availability) &
              static_cast<std::uint8_t>(Castle::BlackKing))
                 ? "k"
                 : "",
             (static_cast<std::uint8_t>(_castling_availability) &
              static_cast<std::uint8_t>(Castle::BlackQueen))
                 ? "q"
                 : "");
    auto castling_rights = std::string(castling_rights_buf);
    if (castling_rights.empty()) {
        castling_rights = "-";
    }

    const auto fen = piece_placements + " " + active_color + " " +
                     castling_rights + " " +
                     utils::SquareToString(en_passant_square()) + " " +
                     std::to_string(_half_move_clock) + " " +
                     std::to_string(_full_move_number);

    return fen.substr(1, std::string::npos);
}

bool
Board::HasLegalMoves() noexcept {
    return movegen::Legal(*this).size() > 0;
}

bool
Board::IsSquareAttacked(Square sq, Color attacker) const noexcept {
    const auto pawns = _pieces[static_cast<std::size_t>(attacker)]
                              [static_cast<std::size_t>(Piece::Pawn)];
    if (attacks::PAWN_ATTACKS[static_cast<std::size_t>(
            utils::GetOpponent(attacker))][static_cast<std::size_t>(sq)] &
        pawns) {
        return true;
    }

    const auto knights = _pieces[static_cast<std::size_t>(attacker)]
                                [static_cast<std::size_t>(Piece::Knight)];
    if (attacks::KNIGHT_ATTACKS[static_cast<std::size_t>(sq)] & knights) {
        return true;
    }

    const auto king = _pieces[static_cast<std::size_t>(attacker)]
                             [static_cast<std::size_t>(Piece::King)];
    if (attacks::KING_ATTACKS[static_cast<std::size_t>(sq)] & king) {
        return true;
    }

    const auto bishops_queens =
        _pieces[static_cast<std::size_t>(attacker)]
               [static_cast<std::size_t>(Piece::Queen)] |
        _pieces[static_cast<std::size_t>(attacker)]
               [static_cast<std::size_t>(Piece::Bishop)];
    if (attacks::BishopAttacks(
            sq, _occupancies[static_cast<std::size_t>(Color::Both)]) &
        bishops_queens) {
        return true;
    }

    const auto rooks_queens = _pieces[static_cast<std::size_t>(attacker)]
                                     [static_cast<std::size_t>(Piece::Queen)] |
                              _pieces[static_cast<std::size_t>(attacker)]
                                     [static_cast<std::size_t>(Piece::Rook)];
    if (attacks::RookAttacks(
            sq, _occupancies[static_cast<std::size_t>(Color::Both)]) &
        rooks_queens) {
        return true;
    }

    return false;
}

void
Board::Make(Move move) noexcept {
    static constexpr int castling_rights[64] = {
        13, 15, 15, 15, 12, 15, 15, 14, 15, 15, 15, 15, 15, 15, 15, 15,
        15, 15, 15, 15, 15, 15, 15, 15, 15, 15, 15, 15, 15, 15, 15, 15,
        15, 15, 15, 15, 15, 15, 15, 15, 15, 15, 15, 15, 15, 15, 15, 15,
        15, 15, 15, 15, 15, 15, 15, 15, 7,  15, 15, 15, 3,  15, 15, 11,
    };

    const auto from_square = move.FromSquare();
    const auto to_square = move.ToSquare();
    const auto piece_type = move.MovedPiece();
    const auto captured_piece = move.CapturedPiece();
    const auto promoted_piece = move.PromotedPiece();
    const auto is_capture = move.IsCapture();
    const auto is_promotion = move.IsPromotion();
    const auto is_double_push = move.IsDoublePush();
    const auto is_en_passant = move.IsEnPassant();
    const auto is_castle = move.IsCastle();

    const auto pawn_push_en_passant_offset = _active == Color::White ? -8 : 8;

    bitboard::PopBit(_pieces[static_cast<std::size_t>(_active)]
                            [static_cast<std::size_t>(piece_type)],
                     from_square);
    _piece[static_cast<std::size_t>(from_square)] = {Color::Black,
                                                     Piece::Empty};

    // Remove from hash key moved piece
    _hash ^= zobrist::PIECE_KEY[static_cast<std::size_t>(_active)]
                               [static_cast<std::size_t>(piece_type)]
                               [static_cast<std::size_t>(from_square)];

    if (is_en_passant) {
        const auto captured_piece_square = static_cast<Square>(
            static_cast<std::uint8_t>(to_square) + pawn_push_en_passant_offset);
        _piece[static_cast<std::size_t>(captured_piece_square)] = {
            Color::Black, Piece::Empty};
        bitboard::PopBit(_pieces[static_cast<std::size_t>(inactive())]
                                [static_cast<std::size_t>(Piece::Pawn)],
                         captured_piece_square);

        // Remove from hash key captured pawn
        _hash ^=
            zobrist::PIECE_KEY[static_cast<std::size_t>(inactive())]
                              [static_cast<std::size_t>(Piece::Pawn)]
                              [static_cast<std::size_t>(captured_piece_square)];
    } else if (is_capture) {
        bitboard::PopBit(_pieces[static_cast<std::size_t>(inactive())]
                                [static_cast<std::size_t>(captured_piece)],
                         to_square);

        // Remove from hash key captured piece
        _hash ^= zobrist::PIECE_KEY[static_cast<std::size_t>(inactive())]
                                   [static_cast<std::size_t>(captured_piece)]
                                   [static_cast<std::size_t>(to_square)];
    }

    if (is_promotion) {
        _piece[static_cast<std::size_t>(to_square)].type = promoted_piece;
        bitboard::SetBit(_pieces[static_cast<std::size_t>(_active)]
                                [static_cast<std::size_t>(promoted_piece)],
                         to_square);

        // Update hash key with promoted piece
        _hash ^= zobrist::PIECE_KEY[static_cast<std::size_t>(_active)]
                                   [static_cast<std::size_t>(promoted_piece)]
                                   [static_cast<std::size_t>(to_square)];
    } else {
        _piece[static_cast<std::size_t>(to_square)].type = piece_type;
        bitboard::SetBit(_pieces[static_cast<std::size_t>(_active)]
                                [static_cast<std::size_t>(piece_type)],
                         to_square);

        // Update hash key with moved piece
        _hash ^= zobrist::PIECE_KEY[static_cast<std::size_t>(_active)]
                                   [static_cast<std::size_t>(piece_type)]
                                   [static_cast<std::size_t>(to_square)];
    }

    _piece[static_cast<std::size_t>(to_square)].color = _active;

    if (is_castle) {
        Square rook_from_square, rook_to_square;
        if (static_cast<int>(to_square) - static_cast<int>(from_square) > 0) {
            rook_from_square =
                _active == Color::White ? Square::H1 : Square::H8;
            rook_to_square = _active == Color::White ? Square::F1 : Square::F8;
        } else {
            rook_from_square =
                _active == Color::White ? Square::A1 : Square::A8;
            rook_to_square = _active == Color::White ? Square::D1 : Square::D8;
        }

        _piece[static_cast<std::size_t>(rook_to_square)] = {_active,
                                                            Piece::Rook};
        _piece[static_cast<std::size_t>(rook_from_square)] = {
            Color::Black, Piece::Empty};

        bitboard::PopBit(_pieces[static_cast<std::size_t>(_active)]
                                [static_cast<std::size_t>(Piece::Rook)],
                         rook_from_square);

        // Remove from hash key rook
        _hash ^= zobrist::PIECE_KEY[static_cast<std::size_t>(_active)]
                                   [static_cast<std::size_t>(Piece::Rook)]
                                   [static_cast<std::size_t>(rook_from_square)];

        bitboard::SetBit(_pieces[static_cast<std::size_t>(_active)]
                                [static_cast<std::size_t>(Piece::Rook)],
                         rook_to_square);

        // Update hash key with rook
        _hash ^= zobrist::PIECE_KEY[static_cast<std::size_t>(_active)]
                                   [static_cast<std::size_t>(Piece::Rook)]
                                   [static_cast<std::size_t>(rook_to_square)];
    }

    // Remove from hash key en passant square
    if (_en_passant_square != Square::Empty) {
        _hash ^= zobrist::EN_PASSANT_KEY[static_cast<std::size_t>(
            _en_passant_square)];
    }

    // Remove from hash key castling_availability rights
    _hash ^=
        zobrist::CASTLE_KEY[static_cast<std::size_t>(_castling_availability)];

    _en_passant_square =
        is_double_push
            ? static_cast<Square>(static_cast<std::uint8_t>(to_square) +
                                  pawn_push_en_passant_offset)
            : Square::Empty;

    _castling_availability = static_cast<Castle>(
        static_cast<std::uint8_t>(_castling_availability) &
        castling_rights[static_cast<std::size_t>(from_square)]);
    _castling_availability = static_cast<Castle>(
        static_cast<std::uint8_t>(_castling_availability) &
        castling_rights[static_cast<std::size_t>(to_square)]);

    // Update hash key with en passant square
    if (_en_passant_square != Square::Empty) {
        _hash ^= zobrist::EN_PASSANT_KEY[static_cast<std::size_t>(
            _en_passant_square)];
    }

    // Update hash key with castling_availability rights
    _hash ^=
        zobrist::CASTLE_KEY[static_cast<std::size_t>(_castling_availability)];

    if (piece_type == Piece::Pawn or is_capture) {
        _half_move_clock = 0;
    } else {
        _half_move_clock++;
    }

    if (_active == Color::Black) {
        _full_move_number++;
    }

    // Remove (and Update) from hash key side to move
    // This works because zobrist::SIDE_KEY[WHITE] = 0
    _hash ^= zobrist::SIDE_KEY[static_cast<std::size_t>(Color::Black)];

    SwitchActive();

    UpdateOccupancies();
}

void
Board::Unmake(const Move move, const StateBackup &backup) noexcept {
    SwitchActive();

    const auto from_square = move.FromSquare();
    const auto to_square = move.ToSquare();
    const auto piece_type = move.MovedPiece();
    const auto captured_piece = move.CapturedPiece();
    const auto promoted_piece = move.PromotedPiece();
    const auto is_capture = move.IsCapture();
    const auto is_promotion = move.IsPromotion();
    const auto is_en_passant = move.IsEnPassant();
    const auto is_castle = move.IsCastle();

    _piece[static_cast<std::size_t>(from_square)].type = piece_type;
    _piece[static_cast<std::size_t>(from_square)].color = _active;
    bitboard::SetBit(_pieces[static_cast<std::size_t>(_active)]
                            [static_cast<std::size_t>(piece_type)],
                     from_square);

    bitboard::PopBit(_pieces[static_cast<std::size_t>(_active)]
                            [static_cast<std::size_t>(piece_type)],
                     to_square);

    if (is_en_passant) {
        const auto captured_piece_square =
            _active == Color::White
                ? static_cast<Square>(static_cast<std::uint8_t>(to_square) - 8)
                : static_cast<Square>(static_cast<std::uint8_t>(to_square) + 8);

        _piece[static_cast<std::size_t>(captured_piece_square)].type =
            Piece::Pawn;
        _piece[static_cast<std::size_t>(captured_piece_square)].color =
            inactive();
        bitboard::SetBit(_pieces[static_cast<std::size_t>(inactive())]
                                [static_cast<std::size_t>(Piece::Pawn)],
                         captured_piece_square);

        _piece[static_cast<std::size_t>(to_square)] = {Color::Black,
                                                       Piece::Empty};
    } else if (is_capture) {
        _piece[static_cast<std::size_t>(to_square)].type = captured_piece;
        _piece[static_cast<std::size_t>(to_square)].color = inactive();
        bitboard::SetBit(_pieces[static_cast<std::size_t>(inactive())]
                                [static_cast<std::size_t>(captured_piece)],
                         to_square);
    } else {
        _piece[static_cast<std::size_t>(to_square)] = {Color::Black,
                                                       Piece::Empty};
    }

    if (is_promotion) {
        bitboard::PopBit(_pieces[static_cast<std::size_t>(_active)]
                                [static_cast<std::size_t>(promoted_piece)],
                         to_square);
    }

    if (is_castle) {
        Square rook_from_square, rook_to_square;
        if (static_cast<int>(to_square) - static_cast<int>(from_square) > 0) {
            rook_from_square =
                _active == Color::White ? Square::H1 : Square::H8;
            rook_to_square = _active == Color::White ? Square::F1 : Square::F8;
        } else {
            rook_from_square =
                _active == Color::White ? Square::A1 : Square::A8;
            rook_to_square = _active == Color::White ? Square::D1 : Square::D8;
        }

        _piece[static_cast<std::size_t>(rook_to_square)] = {Color::Black,
                                                            Piece::Empty};
        bitboard::PopBit(_pieces[static_cast<std::size_t>(_active)]
                                [static_cast<std::size_t>(Piece::Rook)],
                         rook_to_square);

        _piece[static_cast<std::size_t>(rook_from_square)] = {_active,
                                                              Piece::Rook};
        bitboard::SetBit(_pieces[static_cast<std::size_t>(_active)]
                                [static_cast<std::size_t>(Piece::Rook)],
                         rook_from_square);
    }

    if (_active == Color::Black) {
        _full_move_number--;
    }

    _hash = backup.hash;
    _half_move_clock = backup.half_move_clock;
    _full_move_number = backup.full_move_number;
    _en_passant_square = backup.en_passant_square;
    _castling_availability = backup.castling_availability;

    UpdateOccupancies();
}

void
Board::FromFen(const std::string &fen_str) noexcept {
    const auto fen = Fen(fen_str);

    for (int i = static_cast<int>(Square::A1);
         i < static_cast<int>(Square::Total); ++i) {
        _piece[i] = {Color::Black, Piece::Empty};
    }

    int file = static_cast<int>(File::FA),
        rank = static_cast<int>(Rank::R8);
    for (const char &c : fen.position) {
        switch (c) {
        case 'p':
            _piece[static_cast<std::size_t>(utils::GetSquare(
                       static_cast<Rank>(rank), static_cast<File>(file)))]
                .type = Piece::Pawn;
            _piece[static_cast<std::size_t>(utils::GetSquare(
                       static_cast<Rank>(rank), static_cast<File>(file)))]
                .color = Color::Black;
            file = (file + 1) % 8;
            break;
        case 'n':
            _piece[static_cast<std::size_t>(utils::GetSquare(
                       static_cast<Rank>(rank), static_cast<File>(file)))]
                .type = Piece::Knight;
            _piece[static_cast<std::size_t>(utils::GetSquare(
                       static_cast<Rank>(rank), static_cast<File>(file)))]
                .color = Color::Black;
            file = (file + 1) % 8;
            break;
        case 'b':
            _piece[static_cast<std::size_t>(utils::GetSquare(
                       static_cast<Rank>(rank), static_cast<File>(file)))]
                .type = Piece::Bishop;
            _piece[static_cast<std::size_t>(utils::GetSquare(
                       static_cast<Rank>(rank), static_cast<File>(file)))]
                .color = Color::Black;
            file = (file + 1) % 8;
            break;
        case 'r':
            _piece[static_cast<std::size_t>(utils::GetSquare(
                       static_cast<Rank>(rank), static_cast<File>(file)))]
                .type = Piece::Rook;
            _piece[static_cast<std::size_t>(utils::GetSquare(
                       static_cast<Rank>(rank), static_cast<File>(file)))]
                .color = Color::Black;
            file = (file + 1) % 8;
            break;
        case 'q':
            _piece[static_cast<std::size_t>(utils::GetSquare(
                       static_cast<Rank>(rank), static_cast<File>(file)))]
                .type = Piece::Queen;
            _piece[static_cast<std::size_t>(utils::GetSquare(
                       static_cast<Rank>(rank), static_cast<File>(file)))]
                .color = Color::Black;
            file = (file + 1) % 8;
            break;
        case 'k':
            _piece[static_cast<std::size_t>(utils::GetSquare(
                       static_cast<Rank>(rank), static_cast<File>(file)))]
                .type = Piece::King;
            _piece[static_cast<std::size_t>(utils::GetSquare(
                       static_cast<Rank>(rank), static_cast<File>(file)))]
                .color = Color::Black;
            file = (file + 1) % 8;
            break;
        case 'P':
            _piece[static_cast<std::size_t>(utils::GetSquare(
                       static_cast<Rank>(rank), static_cast<File>(file)))]
                .type = Piece::Pawn;
            _piece[static_cast<std::size_t>(utils::GetSquare(
                       static_cast<Rank>(rank), static_cast<File>(file)))]
                .color = Color::White;
            file = (file + 1) % 8;
            break;
        case 'N':
            _piece[static_cast<std::size_t>(utils::GetSquare(
                       static_cast<Rank>(rank), static_cast<File>(file)))]
                .type = Piece::Knight;
            _piece[static_cast<std::size_t>(utils::GetSquare(
                       static_cast<Rank>(rank), static_cast<File>(file)))]
                .color = Color::White;
            file = (file + 1) % 8;
            break;
        case 'B':
            _piece[static_cast<std::size_t>(utils::GetSquare(
                       static_cast<Rank>(rank), static_cast<File>(file)))]
                .type = Piece::Bishop;
            _piece[static_cast<std::size_t>(utils::GetSquare(
                       static_cast<Rank>(rank), static_cast<File>(file)))]
                .color = Color::White;
            file = (file + 1) % 8;
            break;
        case 'R':
            _piece[static_cast<std::size_t>(utils::GetSquare(
                       static_cast<Rank>(rank), static_cast<File>(file)))]
                .type = Piece::Rook;
            _piece[static_cast<std::size_t>(utils::GetSquare(
                       static_cast<Rank>(rank), static_cast<File>(file)))]
                .color = Color::White;
            file = (file + 1) % 8;
            break;
        case 'Q':
            _piece[static_cast<std::size_t>(utils::GetSquare(
                       static_cast<Rank>(rank), static_cast<File>(file)))]
                .type = Piece::Queen;
            _piece[static_cast<std::size_t>(utils::GetSquare(
                       static_cast<Rank>(rank), static_cast<File>(file)))]
                .color = Color::White;
            file = (file + 1) % 8;
            break;
        case 'K':
            _piece[static_cast<std::size_t>(utils::GetSquare(
                       static_cast<Rank>(rank), static_cast<File>(file)))]
                .type = Piece::King;
            _piece[static_cast<std::size_t>(utils::GetSquare(
                       static_cast<Rank>(rank), static_cast<File>(file)))]
                .color = Color::White;
            file = (file + 1) % 8;
            break;
        case '/':
            rank--;
            file = 0;
            break;
        default:
            file += (c - '0');
            break;
        }
    }

    if (fen.active == 'w') {
        _active = Color::White;
    } else {
        _active = Color::Black;
    }

    for (const char &c : fen.castling_availability) {
        switch (c) {
        case 'Q':
            _castling_availability = static_cast<Castle>(
                static_cast<std::uint8_t>(_castling_availability) +
                static_cast<std::uint8_t>(Castle::WhiteQueen));
            break;
        case 'K':
            _castling_availability = static_cast<Castle>(
                static_cast<std::uint8_t>(_castling_availability) +
                static_cast<std::uint8_t>(Castle::WhiteKing));
            break;
        case 'q':
            _castling_availability = static_cast<Castle>(
                static_cast<std::uint8_t>(_castling_availability) +
                static_cast<std::uint8_t>(Castle::BlackQueen));
            break;
        case 'k':
            _castling_availability = static_cast<Castle>(
                static_cast<std::uint8_t>(_castling_availability) +
                static_cast<std::uint8_t>(Castle::BlackKing));
            break;
        default:
            break;
        }
    }

    if (fen.en_passant_square != "-") {
        const int en_passant_file = fen.en_passant_square[0] - 'a';
        const int en_passant_rank = fen.en_passant_square[1] - '1';
        _en_passant_square =
            utils::GetSquare(static_cast<Rank>(en_passant_rank),
                             static_cast<File>(en_passant_file));
    } else {
        _en_passant_square = Square::Empty;
    }

    _half_move_clock = fen.half_move_clock;

    _full_move_number = fen.full_move_number;

    UpdateBitboards();

    _hash = zobrist::Hash(*this);
}

void
Board::UpdateOccupancies() noexcept {
    // Reset occupancies
    _occupancies[static_cast<std::size_t>(Color::Both)] = bitboard::ZERO;
    _occupancies[static_cast<std::size_t>(Color::White)] = bitboard::ZERO;
    _occupancies[static_cast<std::size_t>(Color::Black)] = bitboard::ZERO;

    // Update white occupancies
    _occupancies[static_cast<std::size_t>(Color::White)] |=
        _pieces[static_cast<std::size_t>(Color::White)]
               [static_cast<std::size_t>(Piece::Pawn)];
    _occupancies[static_cast<std::size_t>(Color::White)] |=
        _pieces[static_cast<std::size_t>(Color::White)]
               [static_cast<std::size_t>(Piece::Knight)];
    _occupancies[static_cast<std::size_t>(Color::White)] |=
        _pieces[static_cast<std::size_t>(Color::White)]
               [static_cast<std::size_t>(Piece::Bishop)];
    _occupancies[static_cast<std::size_t>(Color::White)] |=
        _pieces[static_cast<std::size_t>(Color::White)]
               [static_cast<std::size_t>(Piece::Rook)];
    _occupancies[static_cast<std::size_t>(Color::White)] |=
        _pieces[static_cast<std::size_t>(Color::White)]
               [static_cast<std::size_t>(Piece::Queen)];
    _occupancies[static_cast<std::size_t>(Color::White)] |=
        _pieces[static_cast<std::size_t>(Color::White)]
               [static_cast<std::size_t>(Piece::King)];

    // Update black occupancies
    _occupancies[static_cast<std::size_t>(Color::Black)] |=
        _pieces[static_cast<std::size_t>(Color::Black)]
               [static_cast<std::size_t>(Piece::Pawn)];
    _occupancies[static_cast<std::size_t>(Color::Black)] |=
        _pieces[static_cast<std::size_t>(Color::Black)]
               [static_cast<std::size_t>(Piece::Knight)];
    _occupancies[static_cast<std::size_t>(Color::Black)] |=
        _pieces[static_cast<std::size_t>(Color::Black)]
               [static_cast<std::size_t>(Piece::Bishop)];
    _occupancies[static_cast<std::size_t>(Color::Black)] |=
        _pieces[static_cast<std::size_t>(Color::Black)]
               [static_cast<std::size_t>(Piece::Rook)];
    _occupancies[static_cast<std::size_t>(Color::Black)] |=
        _pieces[static_cast<std::size_t>(Color::Black)]
               [static_cast<std::size_t>(Piece::Queen)];
    _occupancies[static_cast<std::size_t>(Color::Black)] |=
        _pieces[static_cast<std::size_t>(Color::Black)]
               [static_cast<std::size_t>(Piece::King)];

    // Update occupancies for both sides
    _occupancies[static_cast<std::size_t>(Color::Both)] |=
        _occupancies[static_cast<std::size_t>(Color::White)];
    _occupancies[static_cast<std::size_t>(Color::Both)] |=
        _occupancies[static_cast<std::size_t>(Color::Black)];
}

void
Board::UpdateBitboards() noexcept {
    // Reset bitboards
    for (int piece_type = static_cast<int>(Piece::Pawn);
         piece_type < static_cast<int>(Piece::Total); ++piece_type) {
        _pieces[static_cast<std::size_t>(Color::White)][piece_type] =
            bitboard::ZERO;
        _pieces[static_cast<std::size_t>(Color::Black)][piece_type] =
            bitboard::ZERO;
    }

    // Update bitboards
    for (int sq = static_cast<int>(Square::A1);
         sq < static_cast<int>(Square::Total); ++sq) {
        if (_piece[sq].type != Piece::Empty) {
            const auto color = _piece[sq].color;
            const auto type = _piece[sq].type;
            bitboard::SetBit(_pieces[static_cast<std::size_t>(color)]
                                    [static_cast<std::size_t>(type)],
                             static_cast<Square>(sq));
        }
    }

    // Update occupancies
    UpdateOccupancies();
}

void
Board::Display(std::ostream &os, const bool ascii,
               const bool white_on_bottom) const noexcept {
    // TODO: some error here when white on top
    if (!white_on_bottom) {
        os << "      h   g   f   e   d   c   b   a\n";
        for (int rank = static_cast<int>(Rank::R1);
             rank < static_cast<int>(Rank::R8); rank++) {
            os << "    +---+---+---+---+---+---+---+---+\n"
               << "    |";
            for (int file = static_cast<int>(File::FH);
                 file >= static_cast<int>(File::FA); file--) {
                const auto piece =
                    _piece[static_cast<std::size_t>(utils::GetSquare(
                        static_cast<Rank>(rank), static_cast<File>(file)))];
                os << " "
                   << utils::PieceToString(piece.type, piece.color, ascii)
                   << " |";
            }
            os << std::setw(3) << rank + 1 << "\n";
        }
        os << "    +---+---+---+---+---+---+---+---+\n";
    } else {
        for (int rank = static_cast<int>(Rank::R8);
             rank >= static_cast<int>(Rank::R1); rank--) {
            os << "    +---+---+---+---+---+---+---+---+\n"
               << std::setw(3) << rank + 1 << " |";
            for (int file = 0; file < 8; file++) {
                const auto piece =
                    _piece[static_cast<std::size_t>(utils::GetSquare(
                        static_cast<Rank>(rank), static_cast<File>(file)))];
                os << " "
                   << utils::PieceToString(piece.type, piece.color, ascii)
                   << " |";
            }
            os << '\n';
        }
        os << "    +---+---+---+---+---+---+---+---+\n"
           << "      a   b   c   d   e   f   g   h\n";
    }

    auto fen = ToFen();
    fen = fen.substr(fen.find(' ') + 1);
    os << "\n    " << fen << "\n";
}
}   // namespace cod::chess