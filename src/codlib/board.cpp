#include <codlib/board.hpp>

#include "codlib/zobrist.hpp"
#include <codlib/bitboard.hpp>
#include <codlib/move.hpp>
#include <codlib/movegen/attacks.hpp>
#include <codlib/utils.hpp>

using bitboard::u64;

[[maybe_unused]] std::string Board::get_fen() const noexcept {
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
            switch (_square[sq].type) {
                case EMPTY_PIECE:
                    empty_squares++;
                    break;
                default:
                    if (empty_squares) {
                        piece_placements += std::to_string(empty_squares);
                        empty_squares = 0;
                    }
                    piece_placements += PIECE_REPR[_square[sq].type + (6 * _square[sq].color)];
                    break;
            }
        }
        if (empty_squares) {
            piece_placements += std::to_string(empty_squares);
            empty_squares = 0;
        }
    }

    active_color = _to_move == WHITE ? "w" : "b";

    char castling_rights_buf[5];
    snprintf(castling_rights_buf,
             5,
             "%s%s%s%s",
             (_castling_rights & CASTLE_KING_WHITE) ? "K" : "",
             (_castling_rights & CASTLE_QUEEN_WHITE) ? "Q" : "",
             (_castling_rights & CASTLE_KING_BLACK) ? "k" : "",
             (_castling_rights & CASTLE_QUEEN_BLACK) ? "q" : "");
    castling_rights = std::string(castling_rights_buf);
    if (castling_rights.empty()) {
        castling_rights = "-";
    }

    std::string fen = piece_placements +
                      " " +
                      active_color +
                      " " +
                      castling_rights +
                      " " +
                      SQUARE_NAMES[get_en_passant_square() == -1 ? 64 : get_en_passant_square()] +
                      " " +
                      std::to_string(_half_move_clock) + " " +
                      std::to_string(_full_move_number) + "\n";

    return fen.substr(1, std::string::npos);
}

bool Board::is_square_attacked(Square sq, Color attacker) const noexcept {
    u64 pawns = _pieces[attacker][PAWN];
    if (attacks::ATTACKS_PAWN[utils::get_opponent(attacker)][sq] & pawns) {
        return true;
    }

    u64 knights = _pieces[attacker][KNIGHT];
    if (attacks::ATTACKS_KNIGHT[sq] & knights) {
        return true;
    }

    u64 king = _pieces[attacker][KING];
    if (attacks::ATTACKS_KING[sq] & king) {
        return true;
    }

    u64 bishopsQueens = _pieces[attacker][QUEEN] | _pieces[attacker][BISHOP];
    if (attacks::get_bishop_attacks(sq, _occupancies[BOTH]) & bishopsQueens) {
        return true;
    }

    u64 rooksQueens = _pieces[attacker][QUEEN] | _pieces[attacker][ROOK];
    if (attacks::get_rook_attacks(sq, _occupancies[BOTH]) & rooksQueens) {
        return true;
    }

    return false;
}

void Board::make(const Move move) noexcept {
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

    int pawn_push_en_passant_offset = _to_move == WHITE ? -8 : 8;

    bitboard::pop_bit(_pieces[_to_move][piece_type], (Square) from_square);
    _square[from_square].type = EMPTY_PIECE;
    _square[from_square].color = BLACK;

    // Remove from hash key moved piece
    _hash_key ^= zobrist::piece_keys[_to_move][piece_type][from_square];

    if (is_en_passant) {
        const auto captured_piece_square = static_cast<Square>(to_square + pawn_push_en_passant_offset);
        _square[captured_piece_square].type = EMPTY_PIECE;
        _square[captured_piece_square].color = BLACK;
        bitboard::pop_bit(_pieces[get_opponent()][PAWN], captured_piece_square);

        // Remove from hash key captured pawn
        _hash_key ^= zobrist::piece_keys[get_opponent()][PAWN][captured_piece_square];
    } else if (is_capture) {
        bitboard::pop_bit(_pieces[get_opponent()][captured_piece], to_square);

        // Remove from hash key captured piece
        _hash_key ^= zobrist::piece_keys[get_opponent()][captured_piece][to_square];
    }

    if (is_promotion) {
        _square[to_square].type = promoted_piece;
        bitboard::set_bit(_pieces[_to_move][promoted_piece], to_square);

        // Update hash key with promoted piece
        _hash_key ^= zobrist::piece_keys[_to_move][promoted_piece][to_square];
    } else {
        _square[to_square].type = piece_type;
        bitboard::set_bit(_pieces[_to_move][piece_type], to_square);

        // Update hash key with moved piece
        _hash_key ^= zobrist::piece_keys[_to_move][piece_type][to_square];
    }

    _square[to_square].color = _to_move;

    if (is_castle) {
        Square rook_from_square, rook_to_square;
        if (to_square - from_square > 0) {
            rook_from_square = _to_move == WHITE ? H1 : H8;
            rook_to_square = _to_move == WHITE ? F1 : F8;
        } else {
            rook_from_square = _to_move == WHITE ? A1 : A8;
            rook_to_square = _to_move == WHITE ? D1 : D8;
        }

        _square[rook_from_square].type = EMPTY_PIECE;
        _square[rook_from_square].color = BLACK;
        _square[rook_to_square].type = ROOK;
        _square[rook_to_square].color = _to_move;

        bitboard::pop_bit(_pieces[_to_move][ROOK], rook_from_square);

        // Remove from hash key rook
        _hash_key ^= zobrist::piece_keys[_to_move][ROOK][rook_from_square];

        bitboard::set_bit(_pieces[_to_move][ROOK], rook_to_square);

        // Update hash key with rook
        _hash_key ^= zobrist::piece_keys[_to_move][ROOK][rook_to_square];
    }

    // Remove from hash key en passant square
    if (_en_passant_square != EMPTY_SQUARE) {
        _hash_key ^= zobrist::en_passant_keys[_en_passant_square];
    }

    // Remove from hash key castling rights
    _hash_key ^= zobrist::castle_keys[_castling_rights];

    _en_passant_square = is_double_push ? (Square) (to_square + pawn_push_en_passant_offset) : EMPTY_SQUARE;
    _castling_rights &= castling_rights[from_square];
    _castling_rights &= castling_rights[to_square];

    // Update hash key with en passant square
    if (_en_passant_square != EMPTY_SQUARE) {
        _hash_key ^= zobrist::en_passant_keys[_en_passant_square];
    }

    // Update hash key with castling rights
    _hash_key ^= zobrist::castle_keys[_castling_rights];

    if (piece_type == PAWN || (is_capture)) {
        _half_move_clock = 0;
    } else {
        _half_move_clock++;
    }

    if (_to_move == BLACK) {
        _full_move_number++;
    }

    // Remove (and Update) from hash key side to move
    // This works because zobrist::side_key[WHITE] = 0
    // So XOR side[BLACK] + XOR side[WHITE] = XOR side[WHITE] + side[BLACK] = XOR side[BLACK]
    _hash_key ^= zobrist::side_key[BLACK];

    // Update from hash key new side to move
    switch_side_to_move();

    update_occupancies();
}

void Board::unmake(const Move move, const GameState state) noexcept {
    switch_side_to_move();

    Square from_square = move.get_from_square();
    Square to_square = move.get_to_square();
    PieceType piece_type = move.get_piece_type();
    PieceType captured_piece = move.get_captured_piece_type();
    PieceType promoted_piece = move.get_promoted_piece_type();
    bool is_capture = move.is_capture();
    bool is_promotion = move.is_promotion();
    bool is_en_passant = move.is_en_passant();
    bool is_castle = move.is_castle();

    _square[from_square].type = piece_type;
    _square[from_square].color = _to_move;
    bitboard::set_bit(_pieces[_to_move][piece_type], from_square);

    bitboard::pop_bit(_pieces[_to_move][piece_type], to_square);

    if (is_en_passant) {
        Square captured_piece_square = _to_move == WHITE ? (Square) (to_square - 8) : (Square) (to_square + 8);

        _square[captured_piece_square].type = PAWN;
        _square[captured_piece_square].color = get_opponent();
        bitboard::set_bit(_pieces[get_opponent()][PAWN], captured_piece_square);

        _square[to_square].type = EMPTY_PIECE;
        _square[to_square].color = BLACK;
    } else if (is_capture) {
        _square[to_square].type = captured_piece;
        _square[to_square].color = get_opponent();
        bitboard::set_bit(_pieces[get_opponent()][captured_piece], to_square);
    } else {
        _square[to_square].type = EMPTY_PIECE;
        _square[to_square].color = BLACK;
    }

    if (is_promotion) {
        bitboard::pop_bit(_pieces[_to_move][promoted_piece], to_square);
    }

    if (is_castle) {
        Square rook_from_square, rook_to_square;
        if (to_square - from_square > 0) {
            rook_from_square = _to_move == WHITE ? H1 : H8;
            rook_to_square = _to_move == WHITE ? F1 : F8;
        } else {
            rook_from_square = _to_move == WHITE ? A1 : A8;
            rook_to_square = _to_move == WHITE ? D1 : D8;
        }

        _square[rook_to_square].type = EMPTY_PIECE;
        _square[rook_to_square].color = BLACK;
        bitboard::pop_bit(_pieces[_to_move][ROOK], rook_to_square);

        _square[rook_from_square].type = ROOK;
        _square[rook_from_square].color = _to_move;
        bitboard::set_bit(_pieces[_to_move][ROOK], rook_from_square);
    }

    if (_to_move == BLACK) {
        _full_move_number--;
    }

    _en_passant_square = state.en_passant_square;
    _castling_rights = state.castling_rights;
    _half_move_clock = state.half_move_clock;
    _hash_key = state.hash_key;

    update_occupancies();
}

void Board::set_from_fen(const std::string &piece_placements,
                         const std::string &active_color,
                         const std::string &castling_rights,
                         const std::string &en_passant,
                         const std::string &half_move_clock,
                         const std::string &full_move_number) noexcept {
    int file = FILE_A, rank = RANK_8;
    for (const char &c: piece_placements) {
        switch (c) {
            case 'p':
                _square[utils::get_square((Rank) rank, (File) file)].type = PAWN;
                _square[utils::get_square((Rank) rank, (File) file)].color = BLACK;
                file = (file + 1) % 8;
                break;
            case 'n':
                _square[utils::get_square((Rank) rank, (File) file)].type = KNIGHT;
                _square[utils::get_square((Rank) rank, (File) file)].color = BLACK;
                file = (file + 1) % 8;
                break;
            case 'b':
                _square[utils::get_square((Rank) rank, (File) file)].type = BISHOP;
                _square[utils::get_square((Rank) rank, (File) file)].color = BLACK;
                file = (file + 1) % 8;
                break;
            case 'r':
                _square[utils::get_square((Rank) rank, (File) file)].type = ROOK;
                _square[utils::get_square((Rank) rank, (File) file)].color = BLACK;
                file = (file + 1) % 8;
                break;
            case 'q':
                _square[utils::get_square((Rank) rank, (File) file)].type = QUEEN;
                _square[utils::get_square((Rank) rank, (File) file)].color = BLACK;
                file = (file + 1) % 8;
                break;
            case 'k':
                _square[utils::get_square((Rank) rank, (File) file)].type = KING;
                _square[utils::get_square((Rank) rank, (File) file)].color = BLACK;
                file = (file + 1) % 8;
                break;
            case 'P':
                _square[utils::get_square((Rank) rank, (File) file)].type = PAWN;
                _square[utils::get_square((Rank) rank, (File) file)].color = WHITE;
                file = (file + 1) % 8;
                break;
            case 'N':
                _square[utils::get_square((Rank) rank, (File) file)].type = KNIGHT;
                _square[utils::get_square((Rank) rank, (File) file)].color = WHITE;
                file = (file + 1) % 8;
                break;
            case 'B':
                _square[utils::get_square((Rank) rank, (File) file)].type = BISHOP;
                _square[utils::get_square((Rank) rank, (File) file)].color = WHITE;
                file = (file + 1) % 8;
                break;
            case 'R':
                _square[utils::get_square((Rank) rank, (File) file)].type = ROOK;
                _square[utils::get_square((Rank) rank, (File) file)].color = WHITE;
                file = (file + 1) % 8;
                break;
            case 'Q':
                _square[utils::get_square((Rank) rank, (File) file)].type = QUEEN;
                _square[utils::get_square((Rank) rank, (File) file)].color = WHITE;
                file = (file + 1) % 8;
                break;
            case 'K':
                _square[utils::get_square((Rank) rank, (File) file)].type = KING;
                _square[utils::get_square((Rank) rank, (File) file)].color = WHITE;
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

    if (active_color == "w") {
        _to_move = WHITE;
    } else {
        _to_move = BLACK;
    }

    for (const char &c: castling_rights) {
        switch (c) {
            case 'Q':
                _castling_rights += CASTLE_QUEEN_WHITE;
                break;
            case 'K':
                _castling_rights += CASTLE_KING_WHITE;
                break;
            case 'q':
                _castling_rights += CASTLE_QUEEN_BLACK;
                break;
            case 'k':
                _castling_rights += CASTLE_KING_BLACK;
                break;
            default:
                break;
        }
    }

    if (en_passant != "-") {
        int en_passant_file = en_passant[0] - 'a';
        int en_passant_rank = en_passant[1] - '1';
        _en_passant_square = utils::get_square((Rank) en_passant_rank, (File) en_passant_file);
    } else {
        _en_passant_square = EMPTY_SQUARE;
    }

    _half_move_clock = std::stoi(half_move_clock);

    _full_move_number = std::stoi(full_move_number);

    update_bitboards_from_squares();

    _hash_key = zobrist::generate_hash_key(*this);
}

std::ostream &operator<<(std::ostream &os, const Board &board) noexcept {
    const int offset = board._ascii ? 0 : 13;

    if (!board._white_on_bottom) {
        os << "      h   g   f   e   d   c   b   a\n";
        for (int rank = RANK_1; rank < RANK_8; rank++) {
            os << "    +---+---+---+---+---+---+---+---+\n"
               << "    |";
            for (int file = FILE_H; file >= FILE_A; file--) {
                Board::Piece piece = board._square[Board::get_square((Rank) rank, (File) file)];
                os << " " << PIECE_REPR[piece.type + offset + (6 * piece.color)] << " |";
            }
            os << std::setw(3) << rank + 1 << "\n";
        }
        os << "    +---+---+---+---+---+---+---+---+\n";
    } else {
        for (int rank = RANK_8; rank >= RANK_1; rank--) {
            os << "    +---+---+---+---+---+---+---+---+\n"
               << std::setw(3) << rank + 1 << " |";

            for (int file = 0; file < 8; file++) {
                Board::Piece piece = board._square[Board::get_square((Rank) rank, (File) file)];
                os << " " << PIECE_REPR[piece.type + offset + (6 * piece.color)] << " |";
            }
            os << '\n';
        }
        os << "    +---+---+---+---+---+---+---+---+\n"
           << "      a   b   c   d   e   f   g   h\n";
    }

    os << "Side to move: " << (board._to_move == WHITE ? "White" : "Black") << '\n';
    os << "Castling rights: " << board.get_castling_rights() << '\n';
    os << "En passant square: " << board.get_en_passant_square() << '\n';
    os << "Half move clock: " << board.get_half_move_clock() << '\n';
    os << "Full move number: " << board.get_full_move_number() << '\n';

    return os;
}

constexpr bool operator==(const Board &b1, const Board &b2) noexcept {
    if (b1.get_side_to_move() != b2.get_side_to_move()) {
        return false;
    }
    if (b1.get_castling_rights() != b2.get_castling_rights()) {
        return false;
    }
    if (b1.get_en_passant_square() != b2.get_en_passant_square()) {
        return false;
    }
    if (b1.get_half_move_clock() != b2.get_half_move_clock()) {
        return false;
    }
    if (b1.get_full_move_number() != b2.get_full_move_number()) {
        return false;
    }
    if (b1.is_ascii() != b2.is_ascii()) {
        return false;
    }
    if (b1.is_white_on_bottom() != b2.is_white_on_bottom()) {
        return false;
    }

    for (int side = 0; side < N_SIDES; side++) {
        for (int piece = 0; piece < N_PIECES; piece++) {
            if (b1.get_pieces(static_cast<Color>(side), static_cast<PieceType>(piece)) !=
                b2.get_pieces(static_cast<Color>(side), static_cast<PieceType>(piece))) {
                return false;
            }
        }
    }

    for (int side = 0; side < N_SIDES + 1; side++) {
        if (b1.get_occupancies(static_cast<Color>(side)) != b2.get_occupancies(static_cast<Color>(side))) {
            return false;
        }
    }

    for (int square = 0; square < N_SQUARES; square++) {
        if (b1.get_piece_from_square(static_cast<Square>(square)) !=
            b2.get_piece_from_square(static_cast<Square>(square))) {
            return false;
        }
    }

    return true;
}

void Board::update_occupancies() noexcept {
    _occupancies[BOTH] = bitboard::kZERO;
    _occupancies[WHITE] = bitboard::kZERO;
    _occupancies[BLACK] = bitboard::kZERO;

    _occupancies[WHITE] |= _pieces[WHITE][PAWN];
    _occupancies[WHITE] |= _pieces[WHITE][KNIGHT];
    _occupancies[WHITE] |= _pieces[WHITE][BISHOP];
    _occupancies[WHITE] |= _pieces[WHITE][ROOK];
    _occupancies[WHITE] |= _pieces[WHITE][QUEEN];
    _occupancies[WHITE] |= _pieces[WHITE][KING];

    _occupancies[BLACK] |= _pieces[BLACK][PAWN];
    _occupancies[BLACK] |= _pieces[BLACK][KNIGHT];
    _occupancies[BLACK] |= _pieces[BLACK][BISHOP];
    _occupancies[BLACK] |= _pieces[BLACK][ROOK];
    _occupancies[BLACK] |= _pieces[BLACK][QUEEN];
    _occupancies[BLACK] |= _pieces[BLACK][KING];

    _occupancies[BOTH] |= _occupancies[WHITE];
    _occupancies[BOTH] |= _occupancies[BLACK];
}

void Board::update_bitboards_from_squares() noexcept {
    for (int piece_type = PAWN; piece_type < N_PIECES; ++piece_type) {
        _pieces[WHITE][piece_type] = bitboard::kZERO;
        _pieces[BLACK][piece_type] = bitboard::kZERO;
    }

    for (int sq = A1; sq < N_SQUARES; ++sq) {
        if (_square[sq].type != EMPTY_PIECE) {
            bitboard::set_bit(_pieces[_square[sq].color][_square[sq].type], static_cast<Square>(sq));
        }
    }

    update_occupancies();
}