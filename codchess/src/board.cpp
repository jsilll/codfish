#include <codchess/board.hpp>

#include <codchess/movegen.hpp>

#include "attacks.hpp"
#include "zobrist.hpp"

namespace cod::chess {
[[maybe_unused]] std::string
Board::ToFen() const noexcept {
    int empty_squares{0};
    std::string piece_placements;
    for (int rank = RANK_8; rank >= RANK_1; rank--) {
        for (int file = FILE_A; file < N_FILES; file++) {
            Square sq = utils::GetSquare(static_cast<Rank>(rank),
                                         static_cast<File>(file));
            if (file == 0) {
                if (empty_squares) {
                    piece_placements += std::to_string(empty_squares);
                    empty_squares = 0;
                }
                piece_placements += '/';
            }
            switch (_piece[sq].type) {
            case EMPTY_PIECE:
                empty_squares++;
                break;
            default:
                if (empty_squares) {
                    piece_placements += std::to_string(empty_squares);
                    empty_squares = 0;
                }
                piece_placements +=
                    utils::PieceToString(_piece[sq].type, _piece[sq].color);
                break;
            }
        }
        if (empty_squares) {
            piece_placements += std::to_string(empty_squares);
            empty_squares = 0;
        }
    }

    const auto active_color = _active == WHITE ? "w" : "b";

    char castling_rights_buf[5];
    snprintf(castling_rights_buf, 5, "%s%s%s%s",
             (_castling_availability & WHITE_KING) ? "K" : "",
             (_castling_availability & WHITE_QUEEN) ? "Q" : "",
             (_castling_availability & BLACK_KING) ? "k" : "",
             (_castling_availability & BLACK_QUEEN) ? "q" : "");
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
    const auto pawns = _pieces[attacker][PAWN];
    if (attacks::PAWN_ATTACKS[utils::GetOpponent(attacker)][sq] & pawns) {
        return true;
    }

    const auto knights = _pieces[attacker][KNIGHT];
    if (attacks::KNIGHT_ATTACKS[sq] & knights) {
        return true;
    }

    const auto king = _pieces[attacker][KING];
    if (attacks::KING_ATTACKS[sq] & king) {
        return true;
    }

    const auto bishops_queens =
        _pieces[attacker][QUEEN] | _pieces[attacker][BISHOP];
    if (attacks::BishopAttacks(sq, _occupancies[BOTH]) & bishops_queens) {
        return true;
    }

    const auto rooks_queens =
        _pieces[attacker][QUEEN] | _pieces[attacker][ROOK];
    if (attacks::RookAttacks(sq, _occupancies[BOTH]) & rooks_queens) {
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

    const auto pawn_push_en_passant_offset = _active == WHITE ? -8 : 8;

    bitboard::PopBit(_pieces[_active][piece_type], (Square) from_square);
    _piece[from_square] = {BLACK, EMPTY_PIECE};

    // Remove from hash key moved piece
    _hash ^= zobrist::PIECE_KEY[_active][piece_type][from_square];

    if (is_en_passant) {
        const auto captured_piece_square =
            static_cast<Square>(to_square + pawn_push_en_passant_offset);
        _piece[captured_piece_square] = {BLACK, EMPTY_PIECE};
        bitboard::PopBit(_pieces[inactive()][PAWN], captured_piece_square);

        // Remove from hash key captured pawn
        _hash ^= zobrist::PIECE_KEY[inactive()][PAWN][captured_piece_square];
    } else if (is_capture) {
        bitboard::PopBit(_pieces[inactive()][captured_piece], to_square);

        // Remove from hash key captured piece
        _hash ^= zobrist::PIECE_KEY[inactive()][captured_piece][to_square];
    }

    if (is_promotion) {
        _piece[to_square].type = promoted_piece;
        bitboard::SetBit(_pieces[_active][promoted_piece], to_square);

        // Update hash key with promoted piece
        _hash ^= zobrist::PIECE_KEY[_active][promoted_piece][to_square];
    } else {
        _piece[to_square].type = piece_type;
        bitboard::SetBit(_pieces[_active][piece_type], to_square);

        // Update hash key with moved piece
        _hash ^= zobrist::PIECE_KEY[_active][piece_type][to_square];
    }

    _piece[to_square].color = _active;

    if (is_castle) {
        Square rook_from_square, rook_to_square;
        if (to_square - from_square > 0) {
            rook_from_square = _active == WHITE ? H1 : H8;
            rook_to_square = _active == WHITE ? F1 : F8;
        } else {
            rook_from_square = _active == WHITE ? A1 : A8;
            rook_to_square = _active == WHITE ? D1 : D8;
        }

        _piece[rook_to_square] = {_active, ROOK};
        _piece[rook_from_square] = {BLACK, EMPTY_PIECE};

        bitboard::PopBit(_pieces[_active][ROOK], rook_from_square);

        // Remove from hash key rook
        _hash ^= zobrist::PIECE_KEY[_active][ROOK][rook_from_square];

        bitboard::SetBit(_pieces[_active][ROOK], rook_to_square);

        // Update hash key with rook
        _hash ^= zobrist::PIECE_KEY[_active][ROOK][rook_to_square];
    }

    // Remove from hash key en passant square
    if (_en_passant_square != EMPTY_SQUARE) {
        _hash ^= zobrist::EN_PASSANT_KEY[_en_passant_square];
    }

    // Remove from hash key castling_availability rights
    _hash ^= zobrist::CASTLE_KEY[_castling_availability];

    _en_passant_square =
        is_double_push ? (Square) (to_square + pawn_push_en_passant_offset)
                       : EMPTY_SQUARE;

    _castling_availability &= castling_rights[from_square];
    _castling_availability &= castling_rights[to_square];

    // Update hash key with en passant square
    if (_en_passant_square != EMPTY_SQUARE) {
        _hash ^= zobrist::EN_PASSANT_KEY[_en_passant_square];
    }

    // Update hash key with castling_availability rights
    _hash ^= zobrist::CASTLE_KEY[_castling_availability];

    if (piece_type == PAWN or (is_capture)) {
        _half_move_clock = 0;
    } else {
        _half_move_clock++;
    }

    if (_active == BLACK) {
        _full_move_number++;
    }

    // Remove (and Update) from hash key side to move
    // This works because zobrist::SIDE_KEY[WHITE] = 0
    _hash ^= zobrist::SIDE_KEY[BLACK];

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

    _piece[from_square].type = piece_type;
    _piece[from_square].color = _active;
    bitboard::SetBit(_pieces[_active][piece_type], from_square);

    bitboard::PopBit(_pieces[_active][piece_type], to_square);

    if (is_en_passant) {
        const auto captured_piece_square = _active == WHITE
                                               ? (Square) (to_square - 8)
                                               : (Square) (to_square + 8);

        _piece[captured_piece_square].type = PAWN;
        _piece[captured_piece_square].color = inactive();
        bitboard::SetBit(_pieces[inactive()][PAWN], captured_piece_square);

        _piece[to_square] = {BLACK, EMPTY_PIECE};
    } else if (is_capture) {
        _piece[to_square].type = captured_piece;
        _piece[to_square].color = inactive();
        bitboard::SetBit(_pieces[inactive()][captured_piece], to_square);
    } else {
        _piece[to_square] = {BLACK, EMPTY_PIECE};
    }

    if (is_promotion) {
        bitboard::PopBit(_pieces[_active][promoted_piece], to_square);
    }

    if (is_castle) {
        Square rook_from_square, rook_to_square;
        if (to_square - from_square > 0) {
            rook_from_square = _active == WHITE ? H1 : H8;
            rook_to_square = _active == WHITE ? F1 : F8;
        } else {
            rook_from_square = _active == WHITE ? A1 : A8;
            rook_to_square = _active == WHITE ? D1 : D8;
        }

        _piece[rook_to_square] = {BLACK, EMPTY_PIECE};
        bitboard::PopBit(_pieces[_active][ROOK], rook_to_square);

        _piece[rook_from_square] = {_active, ROOK};
        bitboard::SetBit(_pieces[_active][ROOK], rook_from_square);
    }

    if (_active == BLACK) {
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

    for (int i = A1; i < N_SQUARES; ++i) {
        _piece[i] = {BLACK, EMPTY_PIECE};
    }

    int file = FILE_A, rank = RANK_8;
    for (const char &c : fen.position) {
        switch (c) {
        case 'p':
            _piece[utils::GetSquare(static_cast<Rank>(rank),
                                    static_cast<File>(file))]
                .type = PAWN;
            _piece[utils::GetSquare(static_cast<Rank>(rank),
                                    static_cast<File>(file))]
                .color = BLACK;
            file = (file + 1) % 8;
            break;
        case 'n':
            _piece[utils::GetSquare(static_cast<Rank>(rank),
                                    static_cast<File>(file))]
                .type = KNIGHT;
            _piece[utils::GetSquare(static_cast<Rank>(rank),
                                    static_cast<File>(file))]
                .color = BLACK;
            file = (file + 1) % 8;
            break;
        case 'b':
            _piece[utils::GetSquare(static_cast<Rank>(rank),
                                    static_cast<File>(file))]
                .type = BISHOP;
            _piece[utils::GetSquare(static_cast<Rank>(rank),
                                    static_cast<File>(file))]
                .color = BLACK;
            file = (file + 1) % 8;
            break;
        case 'r':
            _piece[utils::GetSquare(static_cast<Rank>(rank),
                                    static_cast<File>(file))]
                .type = ROOK;
            _piece[utils::GetSquare(static_cast<Rank>(rank),
                                    static_cast<File>(file))]
                .color = BLACK;
            file = (file + 1) % 8;
            break;
        case 'q':
            _piece[utils::GetSquare(static_cast<Rank>(rank),
                                    static_cast<File>(file))]
                .type = QUEEN;
            _piece[utils::GetSquare(static_cast<Rank>(rank),
                                    static_cast<File>(file))]
                .color = BLACK;
            file = (file + 1) % 8;
            break;
        case 'k':
            _piece[utils::GetSquare(static_cast<Rank>(rank),
                                    static_cast<File>(file))]
                .type = KING;
            _piece[utils::GetSquare(static_cast<Rank>(rank),
                                    static_cast<File>(file))]
                .color = BLACK;
            file = (file + 1) % 8;
            break;
        case 'P':
            _piece[utils::GetSquare(static_cast<Rank>(rank),
                                    static_cast<File>(file))]
                .type = PAWN;
            _piece[utils::GetSquare(static_cast<Rank>(rank),
                                    static_cast<File>(file))]
                .color = WHITE;
            file = (file + 1) % 8;
            break;
        case 'N':
            _piece[utils::GetSquare(static_cast<Rank>(rank),
                                    static_cast<File>(file))]
                .type = KNIGHT;
            _piece[utils::GetSquare(static_cast<Rank>(rank),
                                    static_cast<File>(file))]
                .color = WHITE;
            file = (file + 1) % 8;
            break;
        case 'B':
            _piece[utils::GetSquare(static_cast<Rank>(rank),
                                    static_cast<File>(file))]
                .type = BISHOP;
            _piece[utils::GetSquare(static_cast<Rank>(rank),
                                    static_cast<File>(file))]
                .color = WHITE;
            file = (file + 1) % 8;
            break;
        case 'R':
            _piece[utils::GetSquare(static_cast<Rank>(rank),
                                    static_cast<File>(file))]
                .type = ROOK;
            _piece[utils::GetSquare(static_cast<Rank>(rank),
                                    static_cast<File>(file))]
                .color = WHITE;
            file = (file + 1) % 8;
            break;
        case 'Q':
            _piece[utils::GetSquare(static_cast<Rank>(rank),
                                    static_cast<File>(file))]
                .type = QUEEN;
            _piece[utils::GetSquare(static_cast<Rank>(rank),
                                    static_cast<File>(file))]
                .color = WHITE;
            file = (file + 1) % 8;
            break;
        case 'K':
            _piece[utils::GetSquare(static_cast<Rank>(rank),
                                    static_cast<File>(file))]
                .type = KING;
            _piece[utils::GetSquare(static_cast<Rank>(rank),
                                    static_cast<File>(file))]
                .color = WHITE;
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
        _active = WHITE;
    } else {
        _active = BLACK;
    }

    for (const char &c : fen.castling_availability) {
        switch (c) {
        case 'Q':
            _castling_availability += WHITE_QUEEN;
            break;
        case 'K':
            _castling_availability += WHITE_KING;
            break;
        case 'q':
            _castling_availability += BLACK_QUEEN;
            break;
        case 'k':
            _castling_availability += BLACK_KING;
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
        _en_passant_square = EMPTY_SQUARE;
    }

    _half_move_clock = fen.half_move_clock;

    _full_move_number = fen.full_move_number;

    UpdateBitboards();

    _hash = zobrist::Hash(*this);
}

void
Board::UpdateOccupancies() noexcept {
    // Reset occupancies
    _occupancies[BOTH] = bitboard::ZERO;
    _occupancies[WHITE] = bitboard::ZERO;
    _occupancies[BLACK] = bitboard::ZERO;

    // Update white occupancies
    _occupancies[WHITE] |= _pieces[WHITE][PAWN];
    _occupancies[WHITE] |= _pieces[WHITE][KNIGHT];
    _occupancies[WHITE] |= _pieces[WHITE][BISHOP];
    _occupancies[WHITE] |= _pieces[WHITE][ROOK];
    _occupancies[WHITE] |= _pieces[WHITE][QUEEN];
    _occupancies[WHITE] |= _pieces[WHITE][KING];

    // Update black occupancies
    _occupancies[BLACK] |= _pieces[BLACK][PAWN];
    _occupancies[BLACK] |= _pieces[BLACK][KNIGHT];
    _occupancies[BLACK] |= _pieces[BLACK][BISHOP];
    _occupancies[BLACK] |= _pieces[BLACK][ROOK];
    _occupancies[BLACK] |= _pieces[BLACK][QUEEN];
    _occupancies[BLACK] |= _pieces[BLACK][KING];

    // Update occupancies for both sides
    _occupancies[BOTH] |= _occupancies[WHITE];
    _occupancies[BOTH] |= _occupancies[BLACK];
}

void
Board::UpdateBitboards() noexcept {
    // Reset bitboards
    for (int piece_type = PAWN; piece_type < N_PIECES; ++piece_type) {
        _pieces[WHITE][piece_type] = bitboard::ZERO;
        _pieces[BLACK][piece_type] = bitboard::ZERO;
    }

    // Update bitboards
    for (int sq = A1; sq < N_SQUARES; ++sq) {
        if (_piece[sq].type != EMPTY_PIECE) {
            const auto color = _piece[sq].color;
            const auto type = _piece[sq].type;
            bitboard::SetBit(_pieces[color][type], static_cast<Square>(sq));
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
        for (int rank = RANK_1; rank < RANK_8; rank++) {
            os << "    +---+---+---+---+---+---+---+---+\n"
               << "    |";
            for (int file = FILE_H; file >= FILE_A; file--) {
                const auto piece = _piece[utils::GetSquare(
                    static_cast<Rank>(rank), static_cast<File>(file))];
                os << " "
                   << utils::PieceToString(piece.type, piece.color, ascii)
                   << " |";
            }
            os << std::setw(3) << rank + 1 << "\n";
        }
        os << "    +---+---+---+---+---+---+---+---+\n";
    } else {
        for (int rank = RANK_8; rank >= RANK_1; rank--) {
            os << "    +---+---+---+---+---+---+---+---+\n"
               << std::setw(3) << rank + 1 << " |";
            for (int file = 0; file < 8; file++) {
                const auto piece = _piece[utils::GetSquare(
                    static_cast<Rank>(rank), static_cast<File>(file))];
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