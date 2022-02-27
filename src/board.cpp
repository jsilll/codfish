#include "board.hpp"

#include "utils.hpp"
#include "attacks.hpp"
#include "tables.hpp"
#include "magics.hpp"
#include <iomanip>
#include <string>

void Board::print(bool ascii) const
{
    int offset = ascii ? 0 : 13;
    std::cout << '\n';
    if (!_white_on_bottom)
    {
        std::cout << "      h   g   f   e   d   c   b   a\n";
        for (int rank = 0; rank < 8; rank++)
        {
            std::cout << "    +---+---+---+---+---+---+---+---+\n"
                      << "    |";
            for (int file = 7; file >= 0; file--)
            {
                struct Piece piece = _square[Utils::getSquare(rank, file)];
                std::cout << " " << PIECE_REPR[piece.type + offset + (6 * piece.color)] << " |";
            }
            std::cout << std::setw(3) << rank + 1 << "\n";
        }
        std::cout << "    +---+---+---+---+---+---+---+---+\n";
    }
    else
    {
        for (int rank = 7; rank >= 0; rank--)
        {
            std::cout << "    +---+---+---+---+---+---+---+---+\n"
                      << std::setw(3) << rank + 1 << " |";

            for (int file = 0; file < 8; file++)
            {
                struct Piece piece = _square[Utils::getSquare(rank, file)];
                std::cout << " " << PIECE_REPR[piece.type + offset + (6 * piece.color)] << " |";
            }
            std::cout << '\n';
        }
        std::cout << "    +---+---+---+---+---+---+---+---+\n"
                  << "      a   b   c   d   e   f   g   h\n";
    }
    std::cout << std::endl;
}

Board::Board()
{
    this->setStartingPosition();
}

void Board::clear()
{
    for (int color = WHITE; color < BOTH; color++)
    {
        for (int piece_type = PAWN; piece_type < EMPTY; piece_type++)
        {
            _pieces[color][piece_type] = ZERO;
        }

        _occupancies[color] = ZERO;
    }
    _occupancies[BOTH] = ZERO;

    _to_move = WHITE;
    _castling_rights = 0;
    _en_passant_square = -1;
    _half_move_clock = 0;
    _full_move_number = 0;

    _white_on_bottom = true;
    for (int sq = A1; sq < N_SQUARES; sq++)
    {
        _square[sq].type = EMPTY; // needs to have this particular values set for correct printing
        _square[sq].color = BLACK;
    }
}

void Board::setStartingPosition()
{
    this->setFromFen("rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR", "w", "KQkq", "-", "0", "1");
}

void Board::updateBBFromSquares()
{
    for (int piece_type = PAWN; piece_type < EMPTY; piece_type++)
    {
        _pieces[WHITE][piece_type] = ZERO;
        _pieces[BLACK][piece_type] = ZERO;
    }

    for (int sq = A1; sq < N_SQUARES; sq++)
    {
        if (_square[sq].type != EMPTY)
        {
            _pieces[_square[sq].color][_square[sq].type] |= Tables::SQUARE_BB[sq];
        }
    }

    for (int piece_type = PAWN; piece_type < EMPTY; piece_type++)
    {
        _occupancies[WHITE] |= _pieces[WHITE][piece_type];
        _occupancies[BLACK] |= _pieces[BLACK][piece_type];
    }

    _occupancies[BOTH] = _occupancies[WHITE] | _occupancies[BLACK];
}

void Board::setFromFen(std::string piece_placements,
                       std::string active_color,
                       std::string castling_rights,
                       std::string en_passant,
                       std::string half_move_clock,
                       std::string full_move_number)
{
    this->clear();

    // Piece Placements Parsing
    int file = 0, rank = 7;
    for (auto &c : piece_placements)
    {
        switch (c)
        {
        case 'p':
            _square[Utils::getSquare(rank, file)].type = PAWN;
            _square[Utils::getSquare(rank, file)].color = BLACK;
            file = (file + 1) % 8;
            break;
        case 'n':
            _square[Utils::getSquare(rank, file)].type = KNIGHT;
            _square[Utils::getSquare(rank, file)].color = BLACK;
            file = (file + 1) % 8;
            break;
        case 'b':
            _square[Utils::getSquare(rank, file)].type = BISHOP;
            _square[Utils::getSquare(rank, file)].color = BLACK;
            file = (file + 1) % 8;
            break;
        case 'r':
            _square[Utils::getSquare(rank, file)].type = ROOK;
            _square[Utils::getSquare(rank, file)].color = BLACK;
            file = (file + 1) % 8;
            break;
        case 'q':
            _square[Utils::getSquare(rank, file)].type = QUEEN;
            _square[Utils::getSquare(rank, file)].color = BLACK;
            file = (file + 1) % 8;
            break;
        case 'k':
            _square[Utils::getSquare(rank, file)].type = KING;
            _square[Utils::getSquare(rank, file)].color = BLACK;
            file = (file + 1) % 8;
            break;
        case 'P':
            _square[Utils::getSquare(rank, file)].type = PAWN;
            _square[Utils::getSquare(rank, file)].color = WHITE;
            file = (file + 1) % 8;
            break;
        case 'N':
            _square[Utils::getSquare(rank, file)].type = KNIGHT;
            _square[Utils::getSquare(rank, file)].color = WHITE;
            file = (file + 1) % 8;
            break;
        case 'B':
            _square[Utils::getSquare(rank, file)].type = BISHOP;
            _square[Utils::getSquare(rank, file)].color = WHITE;
            file = (file + 1) % 8;
            break;
        case 'R':
            _square[Utils::getSquare(rank, file)].type = ROOK;
            _square[Utils::getSquare(rank, file)].color = WHITE;
            file = (file + 1) % 8;
            break;
        case 'Q':
            _square[Utils::getSquare(rank, file)].type = QUEEN;
            _square[Utils::getSquare(rank, file)].color = WHITE;
            file = (file + 1) % 8;
            break;
        case 'K':
            _square[Utils::getSquare(rank, file)].type = KING;
            _square[Utils::getSquare(rank, file)].color = WHITE;
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

    // Active Color Parsing
    if (active_color == "w")
    {
        _to_move = WHITE;
    }
    else
    {
        _to_move = BLACK;
    }

    // Castling Rights Parsing
    for (auto &c : castling_rights)
    {
        switch (c)
        {
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

    // En Passant Square Parsing
    if (en_passant != "-")
    {
        int en_passant_file = en_passant[0] - 'a';
        int en_passant_rank = en_passant[1] - '1';
        _en_passant_square = Utils::getSquare(en_passant_rank, en_passant_file);
    }
    else
    {
        _en_passant_square = -1;
    }

    // Halfmove Clock Parsing
    _half_move_clock = std::stoi(half_move_clock);

    // Fullmove Number Parsing
    _full_move_number = std::stoi(full_move_number);

    this->updateBBFromSquares();
}

std::string Board::getFen() const
{
    std::string piece_placements;
    std::string active_color;
    std::string castling_rights;
    std::string en_passant;
    std::string half_move_clock;
    std::string full_move_number;

    // Piece Placements
    int empty_squares = 0;
    for (int rank = 7; rank >= 0; rank--)
    {
        for (int file = 0; file < 8; file++)
        {
            int sq = Utils::getSquare(rank, file);
            if (file == 0)
            {
                if (empty_squares)
                {
                    piece_placements += std::to_string(empty_squares);
                    empty_squares = 0;
                }
                piece_placements += '/';
            }
            switch (_square[sq].type)
            {
            case EMPTY:
                empty_squares++;
                break;
            default:
                if (empty_squares)
                {
                    piece_placements += std::to_string(empty_squares);
                    empty_squares = 0;
                }
                piece_placements += PIECE_REPR[_square[sq].type + (6 * _square[sq].color)];
                break;
            }
        }
        if (empty_squares)
        {
            piece_placements += std::to_string(empty_squares);
            empty_squares = 0;
        }
    }

    // Active Color
    active_color = _to_move == WHITE ? "w" : "b";

    // Castling Rights
    char castling_rights_buf[5];
    snprintf(castling_rights_buf, 5, "%s%s%s%s",
             (_castling_rights & CASTLE_KING_WHITE) ? "K" : "",
             (_castling_rights & CASTLE_QUEEN_WHITE) ? "Q" : "",
             (_castling_rights & CASTLE_KING_BLACK) ? "k" : "",
             (_castling_rights & CASTLE_QUEEN_BLACK) ? "q" : "");
    castling_rights = std::string(castling_rights_buf);
    if (castling_rights == "")
    {
        castling_rights = "-";
    }

    // En Passant Square
    std::string fen = piece_placements + " " +
                      active_color + " " +
                      castling_rights + " " +
                      SQUARE_NAMES[this->getEnPassantSquare() == -1 ? 64 : this->getEnPassantSquare()] + " " +
                      std::to_string(_half_move_clock) + " " +  // Halfmove Clock
                      std::to_string(_full_move_number) + "\n"; // Fullmove Number

    return fen.substr(1, std::string::npos);
}

int Board::getCastlingRights() const
{
    return _castling_rights;
}

int Board::getEnPassantSquare() const
{
    return _en_passant_square;
}

int Board::getHalfMoveClock() const
{
    return _half_move_clock;
}

int Board::getFullMoveNumber() const
{
    return _full_move_number;
}

U64 Board::getOccupiedSquares() const
{
    return _occupancies[BOTH];
}

int Board::getColorToMove() const
{
    return _to_move;
}

int Board::switchSideToMove()
{
    return _to_move = (_to_move + 1) % 2;
}

bool Board::rotate()
{
    return _white_on_bottom = !_white_on_bottom;
}

inline U64 getBishopAttacks(const int &sq, U64 occ)
{
    occ &= Magics::MAGIC_TABLE_BISHOP[sq].mask;
    occ *= Magics::MAGIC_TABLE_BISHOP[sq].magic;
    occ >>= Magics::MAGIC_TABLE_BISHOP[sq].shift;
    return Tables::ATTACKS_BISHOP[sq][occ];
}

inline U64 getRookAttacks(const int &sq, U64 occ)
{
    occ &= Magics::MAGIC_TABLE_ROOK[sq].mask;
    occ *= Magics::MAGIC_TABLE_ROOK[sq].magic;
    occ >>= Magics::MAGIC_TABLE_ROOK[sq].shift;
    return Tables::ATTACKS_ROOK[sq][occ];
}

inline U64 getQueenAttacks(const int &sq, U64 occ)
{
    return getBishopAttacks(sq, occ) | getRookAttacks(sq, occ);
}

bool Board::isSquareAttacked(const int sq) const
{
    if (getRookAttacks(sq, _occupancies[BOTH]) & (_pieces[_to_move][ROOK] | _pieces[_to_move][QUEEN]))
    {
        return true;
    }
    else if (getBishopAttacks(sq, _occupancies[BOTH]) & (_pieces[_to_move][BISHOP] | _pieces[_to_move][QUEEN]))
    {
        return true;
    }
    else if (Tables::ATTACKS_KNIGHT[sq] & _pieces[_to_move][KNIGHT])
    {
        return true;
    }
    else if (Tables::ATTACKS_PAWN[BLACK][sq] & _pieces[_to_move][PAWN])
    {
        return true;
    }
    else if (Tables::ATTACKS_KING[sq] & _pieces[_to_move][KING])
    {
        return true;
    }
    return false;
}

// TODO: maybe create a movegen.cpp
void Board::getLegalMoves() const
{
    // Pawn Moves
    int double_offset = _to_move == WHITE ? -16 : 16;
    int single_offset = double_offset / 2;

    // Pawn Double Pushes TODO: implement generalized shift to remove turnery
    U64 pawn_double_pushes = _to_move == WHITE ? Attacks::maskWhitePawnDoublePushes(_pieces[_to_move][PAWN], ~_occupancies[BOTH])
                                               : Attacks::maskBlackPawnDoublePushes(_pieces[_to_move][PAWN], ~_occupancies[BOTH]);
    while (pawn_double_pushes)
    {
        int to_square = Utils::bitScan(pawn_double_pushes);
        int from_square = to_square + double_offset;
        std::cout << SQUARE_NAMES[from_square] << SQUARE_NAMES[to_square] << std::endl;
        Utils::popLastBit(pawn_double_pushes);
    }

    U64 pawn_single_pushes = _to_move == WHITE ? Attacks::maskWhitePawnSinglePushes(_pieces[_to_move][PAWN], ~_occupancies[BOTH])
                                               : Attacks::maskBlackPawnSinglePushes(_pieces[_to_move][PAWN], ~_occupancies[BOTH]);
    // Pawn Single Pushes With Promotion
    U64 pawn_single_pushes_promo = pawn_single_pushes & (Tables::MASK_RANK[0] | Tables::MASK_RANK[7]);
    while (pawn_single_pushes_promo)
    {
        int to_square = Utils::bitScan(pawn_single_pushes_promo);
        int from_square = to_square + single_offset;
        std::cout << SQUARE_NAMES[from_square] << SQUARE_NAMES[to_square] << "k" << std::endl;
        std::cout << SQUARE_NAMES[from_square] << SQUARE_NAMES[to_square] << "b" << std::endl;
        std::cout << SQUARE_NAMES[from_square] << SQUARE_NAMES[to_square] << "r" << std::endl;
        std::cout << SQUARE_NAMES[from_square] << SQUARE_NAMES[to_square] << "q" << std::endl;
        Utils::popLastBit(pawn_single_pushes_promo);
    }

    // Pawn Single Pushes No Promotion
    U64 pawn_single_pushes_no_promo = pawn_single_pushes & Tables::MASK_CLEAR_RANK[0] & Tables::MASK_CLEAR_RANK[7];
    while (pawn_single_pushes_no_promo)
    {
        int to_square = Utils::bitScan(pawn_single_pushes_no_promo);
        int from_square = to_square + single_offset;
        std::cout << SQUARE_NAMES[from_square] << SQUARE_NAMES[to_square] << std::endl;
        Utils::popLastBit(pawn_single_pushes_no_promo);
    }

    // Pawn Captures With Promotion
    U64 pawns_can_capture_with_promo = _pieces[_to_move][PAWN] & Tables::MASK_RANK[6 - (5 * _to_move)];
    while (pawns_can_capture_with_promo)
    {
        int from_square = Utils::bitScan(pawns_can_capture_with_promo);
        U64 pawn_captures_promo = Tables::ATTACKS_PAWN[_to_move][from_square] & _occupancies[(_to_move + 1) % 2];
        while (pawn_captures_promo)
        {
            int to_square = Utils::bitScan(pawn_captures_promo);
            std::cout << SQUARE_NAMES[from_square] << SQUARE_NAMES[to_square] << "k" << std::endl;
            std::cout << SQUARE_NAMES[from_square] << SQUARE_NAMES[to_square] << "b" << std::endl;
            std::cout << SQUARE_NAMES[from_square] << SQUARE_NAMES[to_square] << "r" << std::endl;
            std::cout << SQUARE_NAMES[from_square] << SQUARE_NAMES[to_square] << "q" << std::endl;
            Utils::popLastBit(pawn_captures_promo);
        }
        Utils::popLastBit(pawns_can_capture_with_promo);
    }

    // Pawns Captures No Promotion
    U64 pawns_can_capture_no_promo = _pieces[_to_move][PAWN] & Tables::MASK_CLEAR_RANK[6 - (5 * _to_move)];
    while (pawns_can_capture_no_promo)
    {
        int from_square = Utils::bitScan(pawns_can_capture_no_promo);
        U64 pawn_captures_no_promo = Tables::ATTACKS_PAWN[_to_move][from_square] & _occupancies[(_to_move + 1) % 2];
        while (pawn_captures_no_promo)
        {
            int to_square = Utils::bitScan(pawn_captures_no_promo);
            std::cout << SQUARE_NAMES[from_square] << SQUARE_NAMES[to_square] << std::endl;
            Utils::popLastBit(pawn_captures_no_promo);
        }
        Utils::popLastBit(pawns_can_capture_no_promo);
    }

    // En-Passant Capture TODO: captured piece must not be absolutely pineed (defending king)
    if (_en_passant_square != -1)
    {
        U64 pawns_can_en_passant = Tables::ATTACKS_PAWN[(_to_move + 1) % 2][_en_passant_square] & _pieces[_to_move][PAWN];
        while (pawns_can_en_passant)
        {
            int from_square = Utils::bitScan(pawns_can_en_passant);
            std::cout << SQUARE_NAMES[from_square] << SQUARE_NAMES[_en_passant_square] << std::endl;
            Utils::popLastBit(pawns_can_en_passant);
        }
    }

    // Castling Moves
    // Knight Moves
    // Bishop Moves
    // Rook Moves
    // Queen Moves
}