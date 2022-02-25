#include "board.hpp"

#include "utils.hpp"
#include "tables.hpp"
#include "magics.hpp"
#include <iomanip>
#include <string>

void Board::print(bool ascii) const
{
    static const std::string PIECE_NAMES[26] = {
        "K ", "Q ", "R ", "K ", "B ", "P ", "K*", "Q*", "R*", "K*", "B*", "P*", "  ",
        "♔ ", "♕ ", "♖ ", "♘ ", "♗ ", "♙ ", "♚ ", "♛ ", "♜ ", "♞ ", "♝ ", "♟︎ ", "  "};

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
                std::cout << " " << PIECE_NAMES[_square[Utils::getSquare(rank, file)] + offset] << "|";
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
                std::cout << " " << PIECE_NAMES[_square[Utils::getSquare(rank, file)] + offset] << "|";
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
    _white_king = ZERO;
    _white_queens = ZERO;
    _white_rooks = ZERO;
    _white_bishops = ZERO;
    _white_knights = ZERO;
    _white_pawns = ZERO;
    _black_king = ZERO;
    _black_queens = ZERO;
    _black_rooks = ZERO;
    _black_bishops = ZERO;
    _black_knights = ZERO;
    _black_pawns = ZERO;
    _white_pieces = ZERO;
    _black_pieces = ZERO;
    _occupied_squares = ZERO;

    _white_to_move = true;
    _castling_rights = 0;
    _en_passant_square = -1;
    _half_move_clock = 0;
    _full_move_number = 0;

    for (int i = 0; i < N_SQUARES; i++)
    {
        _square[i] = EMPTY;
    }

    _white_on_bottom = true;
}

void Board::setStartingPosition()
{
    this->setFromFen("rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR", "w", "KQkq", "-", "0", "1");
}

void Board::updateBBFromSquares()
{
    _white_king = ZERO;
    _white_queens = ZERO;
    _white_rooks = ZERO;
    _white_bishops = ZERO;
    _white_knights = ZERO;
    _white_pawns = ZERO;
    _black_king = ZERO;
    _black_queens = ZERO;
    _black_rooks = ZERO;
    _black_bishops = ZERO;
    _black_knights = ZERO;
    _black_pawns = ZERO;
    _white_pieces = ZERO;
    _black_pieces = ZERO;
    _occupied_squares = ZERO;

    for (int i = 0; i < N_SQUARES; i++)
    {
        switch (_square[i])
        {
        case WHITE_KING:
            _white_king = _white_king | Tables::SQUARE_BB[i];
            break;
        case WHITE_QUEEN:
            _white_queens = _white_queens | Tables::SQUARE_BB[i];
            break;
        case WHITE_ROOK:
            _white_rooks = _white_rooks | Tables::SQUARE_BB[i];
            break;
        case WHITE_BISHOP:
            _white_bishops = _white_bishops | Tables::SQUARE_BB[i];
            break;
        case WHITE_KNIGHT:
            _white_knights = _white_knights | Tables::SQUARE_BB[i];
            break;
        case WHITE_PAWN:
            _white_pawns = _white_pawns | Tables::SQUARE_BB[i];
            break;
        case BLACK_KING:
            _black_king = _black_king | Tables::SQUARE_BB[i];
            break;
        case BLACK_QUEEN:
            _black_queens = _black_queens | Tables::SQUARE_BB[i];
            break;
        case BLACK_ROOK:
            _black_rooks = _black_rooks | Tables::SQUARE_BB[i];
            break;
        case BLACK_BISHOP:
            _black_bishops = _black_bishops | Tables::SQUARE_BB[i];
            break;
        case BLACK_KNIGHT:
            _black_knights = _black_knights | Tables::SQUARE_BB[i];
            break;
        case BLACK_PAWN:
            _black_pawns = _black_pawns | Tables::SQUARE_BB[i];
            break;
        default:
            break;
        }
    }

    _white_pieces = _white_king | _white_queens | _white_rooks | _white_bishops | _white_knights | _white_pawns;
    _black_pieces = _black_king | _black_queens | _black_rooks | _black_bishops | _black_knights | _black_pawns;
    _occupied_squares = _white_pieces | _black_pieces;
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
        switch (c) // TODO: improve perfomance by finding a mapping from char to Enum Piece
        {
        case 'p':
            _square[Utils::getSquare(rank, file)] = BLACK_PAWN;
            file = (file + 1) % 8;
            break;
        case 'n':
            _square[Utils::getSquare(rank, file)] = BLACK_KNIGHT;
            file = (file + 1) % 8;
            break;
        case 'b':
            _square[Utils::getSquare(rank, file)] = BLACK_BISHOP;
            file = (file + 1) % 8;
            break;
        case 'r':
            _square[Utils::getSquare(rank, file)] = BLACK_ROOK;
            file = (file + 1) % 8;
            break;
        case 'q':
            _square[Utils::getSquare(rank, file)] = BLACK_QUEEN;
            file = (file + 1) % 8;
            break;
        case 'k':
            _square[Utils::getSquare(rank, file)] = BLACK_KING;
            file = (file + 1) % 8;
            break;
        case 'P':
            _square[Utils::getSquare(rank, file)] = WHITE_PAWN;
            file = (file + 1) % 8;
            break;
        case 'N':
            _square[Utils::getSquare(rank, file)] = WHITE_KNIGHT;
            file = (file + 1) % 8;
            break;
        case 'B':
            _square[Utils::getSquare(rank, file)] = WHITE_BISHOP;
            file = (file + 1) % 8;
            break;
        case 'R':
            _square[Utils::getSquare(rank, file)] = WHITE_ROOK;
            file = (file + 1) % 8;
            break;
        case 'Q':
            _square[Utils::getSquare(rank, file)] = WHITE_QUEEN;
            file = (file + 1) % 8;
            break;
        case 'K':
            _square[Utils::getSquare(rank, file)] = WHITE_KING;
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
        _white_to_move = true;
    }
    else
    {
        _white_to_move = false;
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

    static const char fen_piece_rep[]{
        // TODO: this depends on Enum Piece order, maybe find better implementation
        'K',
        'Q',
        'R',
        'N',
        'B',
        'P',
        'k',
        'q',
        'r',
        'n',
        'b',
        'p',
    };

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
            switch (_square[sq])
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
                piece_placements += fen_piece_rep[_square[sq]];
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
    active_color = _white_to_move ? "w" : "b";

    // Castling Rights
    char castling_rights_buf[5]; // TODO: hanlde other cases
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
    // clang-format off
    static const char* SQUARE_NAMES[] = {
        "a1","b1","c1","d1","e1","f1","g1","h1",
        "a2","b2","c2","d2","e2","f2","g2","h2",
        "a3","b3","c3","d3","e3","f3","g3","h3",
        "a4","b4","c4","d4","e4","f4","g4","h4",
        "a5","b5","c5","d5","e5","f5","g5","h5",
        "a6","b6","c6","d6","e6","f6","g6","h6",
        "a7","b7","c7","d7","e7","f7","g7","h7",
        "a8","b8","c8","d8","e8","f8","g8","h8", 
        "-"
    };
    // clang-format on

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
    return _occupied_squares;
}

bool Board::isWhiteToMove() const
{
    return _white_to_move;
}

bool Board::switchSideToMove()
{
    return _white_to_move = !_white_to_move;
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
    if (_white_to_move) // TODO: maybe split in two separate functions
    {
        if (getRookAttacks(sq, _occupied_squares) & (_white_rooks | _white_queens))
        {
            return true;
        }
        else if (getBishopAttacks(sq, _occupied_squares) & (_white_bishops | _white_queens))
        {
            return true;
        }
        else if (Tables::ATTACKS_KNIGHT[sq] & _white_knights)
        {
            return true;
        }
        else if (Tables::ATTACKS_PAWN[BLACK][sq] & _white_pawns)
        {
            return true;
        }
        else if (Tables::ATTACKS_KING[sq] & _white_king)
        {
            return true;
        }
    }
    else
    {
        if (getRookAttacks(sq, _occupied_squares) & (_black_rooks | _black_queens))
        {
            return true;
        }
        else if (getBishopAttacks(sq, _occupied_squares) & (_black_bishops | _black_queens))
        {
            return true;
        }
        else if (Tables::ATTACKS_KNIGHT[sq] & _black_knights)
        {
            return true;
        }
        else if (Tables::ATTACKS_PAWN[WHITE][sq] & _black_pawns)
        {
            return true;
        }
        else if (Tables::ATTACKS_KING[sq] & _black_king)
        {
            return true;
        }
    }

    return false;
}

void Board::getLegalMoves() const
{
    int src_square, target_square;
    U64 bitboard, attacks;

    if (_white_to_move)
    {
        // generate pawn moves
        // generate castling moves
    }
    else
    {
        // generate pawn moves
        // generate castling moves
    }

    // generate knight moves
    // generate bishop moves
    // generate rook moves
    // generate queen moves
    // generate king moves
}