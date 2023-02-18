#include <codlib/board.hpp>

#include <codlib/zobrist.hpp>
#include <codlib/bitboard.hpp>
#include <codlib/move.hpp>
#include <codlib/movegen/attacks.hpp>
#include <codlib/utils.hpp>
#include <codlib/display.hpp>

using bitboard::Bitboard;

Board::Fen Board::ParseFen(const std::string &fen) noexcept {
    Fen res;
    std::stringstream ss(fen);

    std::getline(ss, res.position, ' ');
    ss >> res.active;
    ss >> res.castling_availability;
    ss >> res.en_passant_square;
    ss >> res.half_move_clock;
    ss >> res.full_move_number;

    return res;
}

[[maybe_unused]] std::string Board::GetFen() const noexcept {
    std::string piece_placements;
    std::string active_color;
    std::string castling_rights;
    std::string en_passant;
    std::string half_move_clock;
    std::string full_move_number;

    int empty_squares = 0;
    for (int rank = RANK_8; rank >= RANK_1; rank--) {
        for (int file = FILE_A; file < N_FILES; file++) {
            Square sq = utils::get_square((Rank) rank, (File) file);
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
                    piece_placements += PIECE_REPR[_piece[sq].type + (6 * _piece[sq].color)];
                    break;
            }
        }
        if (empty_squares) {
            piece_placements += std::to_string(empty_squares);
            empty_squares = 0;
        }
    }

    active_color = _active == WHITE ? "w" : "b";

    char castling_rights_buf[5];
    snprintf(castling_rights_buf,
             5,
             "%s%s%s%s",
             (_castling_availability & WHITE_KING) ? "K" : "",
             (_castling_availability & WHITE_QUEEN) ? "Q" : "",
             (_castling_availability & BLACK_KING) ? "k" : "",
             (_castling_availability & BLACK_QUEEN) ? "q" : "");
    castling_rights = std::string(castling_rights_buf);
    if (castling_rights.empty()) { castling_rights = "-"; }

    std::string fen = piece_placements + " " + active_color + " " + castling_rights + " "
                      + SQUARE_NAMES[en_passant_square() == -1 ? 64 : en_passant_square()] + " "
                      + std::to_string(_half_move_clock) + " " + std::to_string(_full_move_number);

    return fen.substr(1, std::string::npos);
}

bool Board::IsSquareAttacked(Square sq, Color attacker) const noexcept {
    Bitboard pawns = _pieces[attacker][PAWN];
    if (attacks::ATTACKS_PAWN[utils::get_opponent(attacker)][sq] & pawns) { return true; }

    Bitboard knights = _pieces[attacker][KNIGHT];
    if (attacks::ATTACKS_KNIGHT[sq] & knights) { return true; }

    Bitboard king = _pieces[attacker][KING];
    if (attacks::ATTACKS_KING[sq] & king) { return true; }

    Bitboard bishopsQueens = _pieces[attacker][QUEEN] | _pieces[attacker][BISHOP];
    if (attacks::get_bishop_attacks(sq, _occupancies[BOTH]) & bishopsQueens) { return true; }

    Bitboard rooksQueens = _pieces[attacker][QUEEN] | _pieces[attacker][ROOK];
    if (attacks::get_rook_attacks(sq, _occupancies[BOTH]) & rooksQueens) { return true; }

    return false;
}

void Board::Make(Move move) noexcept {
    // clang-format off
    static const int castling_rights[64] = {
            13, 15, 15, 15, 12, 15, 15, 14,
            15, 15, 15, 15, 15, 15, 15, 15,
            15, 15, 15, 15, 15, 15, 15, 15,
            15, 15, 15, 15, 15, 15, 15, 15,
            15, 15, 15, 15, 15, 15, 15, 15,
            15, 15, 15, 15, 15, 15, 15, 15,
            15, 15, 15, 15, 15, 15, 15, 15,
            7, 15, 15, 15, 3, 15, 15, 11,
    };
    // clang-format on

    Square from_square = move.get_from_square();
    Square to_square = move.get_to_square();
    PieceType piece_type = move.get_piece_type();
    PieceType captured_piece = move.get_captured_piece_type();
    PieceType promoted_piece = move.get_promoted_piece_type();
    bool is_capture = move.is_capture();
    bool is_promotion = move.is_promotion();
    bool is_double_push = move.is_double_push();
    bool is_en_passant = move.is_en_passant();
    bool is_castle = move.is_castle();

    int pawn_push_en_passant_offset = _active == WHITE ? -8 : 8;

    bitboard::pop_bit(_pieces[_active][piece_type], (Square) from_square);
    _piece[from_square].type = EMPTY_PIECE;
    _piece[from_square].color = BLACK;

    // Remove from hash key moved piece
    _hash_key ^= zobrist::piece_keys[_active][piece_type][from_square];

    if (is_en_passant) {
        const auto captured_piece_square = static_cast<Square>(to_square + pawn_push_en_passant_offset);
        _piece[captured_piece_square].type = EMPTY_PIECE;
        _piece[captured_piece_square].color = BLACK;
        bitboard::pop_bit(_pieces[inactive()][PAWN], captured_piece_square);

        // Remove from hash key captured pawn
        _hash_key ^= zobrist::piece_keys[inactive()][PAWN][captured_piece_square];
    } else if (is_capture) {
        bitboard::pop_bit(_pieces[inactive()][captured_piece], to_square);

        // Remove from hash key captured piece
        _hash_key ^= zobrist::piece_keys[inactive()][captured_piece][to_square];
    }

    if (is_promotion) {
        _piece[to_square].type = promoted_piece;
        bitboard::set_bit(_pieces[_active][promoted_piece], to_square);

        // Update hash key with promoted piece
        _hash_key ^= zobrist::piece_keys[_active][promoted_piece][to_square];
    } else {
        _piece[to_square].type = piece_type;
        bitboard::set_bit(_pieces[_active][piece_type], to_square);

        // Update hash key with moved piece
        _hash_key ^= zobrist::piece_keys[_active][piece_type][to_square];
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

        _piece[rook_from_square].type = EMPTY_PIECE;
        _piece[rook_from_square].color = BLACK;
        _piece[rook_to_square].type = ROOK;
        _piece[rook_to_square].color = _active;

        bitboard::pop_bit(_pieces[_active][ROOK], rook_from_square);

        // Remove from hash key rook
        _hash_key ^= zobrist::piece_keys[_active][ROOK][rook_from_square];

        bitboard::set_bit(_pieces[_active][ROOK], rook_to_square);

        // Update hash key with rook
        _hash_key ^= zobrist::piece_keys[_active][ROOK][rook_to_square];
    }

    // Remove from hash key en passant square
    if (_en_passant_square != EMPTY_SQUARE) { _hash_key ^= zobrist::en_passant_keys[_en_passant_square]; }

    // Remove from hash key castling_availability rights
    _hash_key ^= zobrist::castle_keys[_castling_availability];

    _en_passant_square = is_double_push ? (Square) (to_square + pawn_push_en_passant_offset) : EMPTY_SQUARE;
    _castling_availability &= castling_rights[from_square];
    _castling_availability &= castling_rights[to_square];

    // Update hash key with en passant square
    if (_en_passant_square != EMPTY_SQUARE) { _hash_key ^= zobrist::en_passant_keys[_en_passant_square]; }

    // Update hash key with castling_availability rights
    _hash_key ^= zobrist::castle_keys[_castling_availability];

    if (piece_type == PAWN || (is_capture)) {
        _half_move_clock = 0;
    } else {
        _half_move_clock++;
    }

    if (_active == BLACK) { _full_move_number++; }

    // Remove (and Update) from hash key side to move
    // This works because zobrist::side_key[WHITE] = 0
    // So XOR side[BLACK] + XOR side[WHITE] = XOR side[WHITE] + side[BLACK] = XOR side[BLACK]
    _hash_key ^= zobrist::side_key[BLACK];

    switch_active();

    UpdateOccupancies();
}

void Board::Unmake(Move move, StateBackup info_board) noexcept {
    switch_active();

    Square from_square = move.get_from_square();
    Square to_square = move.get_to_square();
    PieceType piece_type = move.get_piece_type();
    PieceType captured_piece = move.get_captured_piece_type();
    PieceType promoted_piece = move.get_promoted_piece_type();
    bool is_capture = move.is_capture();
    bool is_promotion = move.is_promotion();
    bool is_en_passant = move.is_en_passant();
    bool is_castle = move.is_castle();

    _piece[from_square].type = piece_type;
    _piece[from_square].color = _active;
    bitboard::set_bit(_pieces[_active][piece_type], from_square);

    bitboard::pop_bit(_pieces[_active][piece_type], to_square);

    if (is_en_passant) {
        Square captured_piece_square = _active == WHITE ? (Square) (to_square - 8) : (Square) (to_square + 8);

        _piece[captured_piece_square].type = PAWN;
        _piece[captured_piece_square].color = inactive();
        bitboard::set_bit(_pieces[inactive()][PAWN], captured_piece_square);

        _piece[to_square].type = EMPTY_PIECE;
        _piece[to_square].color = BLACK;
    } else if (is_capture) {
        _piece[to_square].type = captured_piece;
        _piece[to_square].color = inactive();
        bitboard::set_bit(_pieces[inactive()][captured_piece], to_square);
    } else {
        _piece[to_square].type = EMPTY_PIECE;
        _piece[to_square].color = BLACK;
    }

    if (is_promotion) { bitboard::pop_bit(_pieces[_active][promoted_piece], to_square); }

    if (is_castle) {
        Square rook_from_square, rook_to_square;
        if (to_square - from_square > 0) {
            rook_from_square = _active == WHITE ? H1 : H8;
            rook_to_square = _active == WHITE ? F1 : F8;
        } else {
            rook_from_square = _active == WHITE ? A1 : A8;
            rook_to_square = _active == WHITE ? D1 : D8;
        }

        _piece[rook_to_square].type = EMPTY_PIECE;
        _piece[rook_to_square].color = BLACK;
        bitboard::pop_bit(_pieces[_active][ROOK], rook_to_square);

        _piece[rook_from_square].type = ROOK;
        _piece[rook_from_square].color = _active;
        bitboard::set_bit(_pieces[_active][ROOK], rook_from_square);
    }

    if (_active == BLACK) { _full_move_number--; }

    _en_passant_square = info_board.en_passant_square;
    _castling_availability = info_board.castling_rights;
    _half_move_clock = info_board.half_move_clock;
    _hash_key = info_board.hash_key;

    UpdateOccupancies();
}

void Board::SetFromFen(const std::string &fen_str) noexcept {
    // TODO: Check if fen is valid

    const Fen fen = ParseFen(fen_str);

    int file = FILE_A, rank = RANK_8;
    for (const char &c: fen.position) {
        switch (c) {
            case 'p':
                _piece[utils::get_square((Rank) rank, (File) file)].type = PAWN;
                _piece[utils::get_square((Rank) rank, (File) file)].color = BLACK;
                file = (file + 1) % 8;
                break;
            case 'n':
                _piece[utils::get_square((Rank) rank, (File) file)].type = KNIGHT;
                _piece[utils::get_square((Rank) rank, (File) file)].color = BLACK;
                file = (file + 1) % 8;
                break;
            case 'b':
                _piece[utils::get_square((Rank) rank, (File) file)].type = BISHOP;
                _piece[utils::get_square((Rank) rank, (File) file)].color = BLACK;
                file = (file + 1) % 8;
                break;
            case 'r':
                _piece[utils::get_square((Rank) rank, (File) file)].type = ROOK;
                _piece[utils::get_square((Rank) rank, (File) file)].color = BLACK;
                file = (file + 1) % 8;
                break;
            case 'q':
                _piece[utils::get_square((Rank) rank, (File) file)].type = QUEEN;
                _piece[utils::get_square((Rank) rank, (File) file)].color = BLACK;
                file = (file + 1) % 8;
                break;
            case 'k':
                _piece[utils::get_square((Rank) rank, (File) file)].type = KING;
                _piece[utils::get_square((Rank) rank, (File) file)].color = BLACK;
                file = (file + 1) % 8;
                break;
            case 'P':
                _piece[utils::get_square((Rank) rank, (File) file)].type = PAWN;
                _piece[utils::get_square((Rank) rank, (File) file)].color = WHITE;
                file = (file + 1) % 8;
                break;
            case 'N':
                _piece[utils::get_square((Rank) rank, (File) file)].type = KNIGHT;
                _piece[utils::get_square((Rank) rank, (File) file)].color = WHITE;
                file = (file + 1) % 8;
                break;
            case 'B':
                _piece[utils::get_square((Rank) rank, (File) file)].type = BISHOP;
                _piece[utils::get_square((Rank) rank, (File) file)].color = WHITE;
                file = (file + 1) % 8;
                break;
            case 'R':
                _piece[utils::get_square((Rank) rank, (File) file)].type = ROOK;
                _piece[utils::get_square((Rank) rank, (File) file)].color = WHITE;
                file = (file + 1) % 8;
                break;
            case 'Q':
                _piece[utils::get_square((Rank) rank, (File) file)].type = QUEEN;
                _piece[utils::get_square((Rank) rank, (File) file)].color = WHITE;
                file = (file + 1) % 8;
                break;
            case 'K':
                _piece[utils::get_square((Rank) rank, (File) file)].type = KING;
                _piece[utils::get_square((Rank) rank, (File) file)].color = WHITE;
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

    for (const char &c: fen.castling_availability) {
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
        int en_passant_file = fen.en_passant_square[0] - 'a';
        int en_passant_rank = fen.en_passant_square[1] - '1';
        _en_passant_square = utils::get_square((Rank) en_passant_rank, (File) en_passant_file);
    } else {
        _en_passant_square = EMPTY_SQUARE;
    }

    _half_move_clock = fen.half_move_clock;

    _full_move_number = fen.full_move_number;

    UpdateBitboards();

    _hash_key = zobrist::generate_hash_key(*this);
}

constexpr bool operator==(const Board &b1, const Board &b2) noexcept {
    if (b1.active() != b2.active()) { return false; }
    if (b1.castling_availability() != b2.castling_availability()) { return false; }
    if (b1.en_passant_square() != b2.en_passant_square()) { return false; }
    if (b1.half_move_clock() != b2.half_move_clock()) { return false; }
    if (b1.full_move_number() != b2.full_move_number()) { return false; }
    for (int side = 0; side < N_SIDES; side++) {
        for (int piece = 0; piece < N_PIECES; piece++) {
            if (b1.pieces(static_cast<Color>(side), static_cast<PieceType>(piece))
                != b2.pieces(static_cast<Color>(side), static_cast<PieceType>(piece))) {
                return false;
            }
        }
    }

    for (int side = 0; side < N_SIDES + 1; side++) {
        if (b1.occupancies(static_cast<Color>(side)) !=
            b2.occupancies(static_cast<Color>(side))) { return false; }
    }

    for (int square = 0; square < N_SQUARES; square++) {
        if (b1.piece(static_cast<Square>(square))
            != b2.piece(static_cast<Square>(square))) {
            return false;
        }
    }

    return true;
}

void Board::UpdateOccupancies() noexcept {
    // Reset occupancies
    _occupancies[BOTH] = bitboard::kZERO;
    _occupancies[WHITE] = bitboard::kZERO;
    _occupancies[BLACK] = bitboard::kZERO;

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

void Board::UpdateBitboards() noexcept {
    for (int piece_type = PAWN; piece_type < N_PIECES; ++piece_type) {
        _pieces[WHITE][piece_type] = bitboard::kZERO;
        _pieces[BLACK][piece_type] = bitboard::kZERO;
    }

    for (int sq = A1; sq < N_SQUARES; ++sq) {
        if (_piece[sq].type != EMPTY_PIECE) {
            bitboard::set_bit(_pieces[_piece[sq].color][_piece[sq].type], static_cast<Square>(sq));
        }
    }

    UpdateOccupancies();
}

// std::ostream &operator<<(std::ostream &os, const Board &board) noexcept {
//     const int offset = board._ascii ? 0 : 13;
//
//     if (!board._white_on_bottom) {
//         os << "      h   g   f   e   d   c   b   a\n";
//         for (int rank = RANK_1; rank < RANK_8; rank++) {
//             os << "    +---+---+---+---+---+---+---+---+\n"
//                << "    |";
//             for (int file = FILE_H; file >= FILE_A; file--) {
//                 Board::Piece piece = board._piece[Board::get_square((Rank) rank, (File) file)];
//                 os << " " << PIECE_REPR[piece.type + offset + (6 * piece.color)] << " |";
//             }
//             os << std::setw(3) << rank + 1 << "\n";
//         }
//         os << "    +---+---+---+---+---+---+---+---+\n";
//     } else {
//         for (int rank = RANK_8; rank >= RANK_1; rank--) {
//             os << "    +---+---+---+---+---+---+---+---+\n" << std::setw(3) << rank + 1 << " |";
//
//             for (int file = 0; file < 8; file++) {
//                 Board::Piece piece = board._piece[Board::get_square((Rank) rank, (File) file)];
//                 os << " " << PIECE_REPR[piece.type + offset + (6 * piece.color)] << " |";
//             }
//             os << '\n';
//         }
//         os << "    +---+---+---+---+---+---+---+---+\n"
//            << "      a   b   c   d   e   f   g   h\n";
//     }
//
//     os << "Side to move: " << (board._active == WHITE ? "White" : "Black") << '\n';
//     os << "Castling rights: " << board.castling_availability() << '\n';
//     os << "En passant square: " << board.en_passant_square() << '\n';
//     os << "Half move clock: " << board.half_move_clock() << '\n';
//     os << "Full move number: " << board.full_move_number() << '\n';
//
//     return os;
// }
