#include "board.hpp"

#include "tables.hpp"
#include "utils.hpp"
#include <iomanip>
#include <cstring>

void Board::print(bool ascii = false)
{
    static const std::string PIECE_NAMES[26] = {"K ", "Q ", "R ", "K ", "B ", "P ", "K*", "Q*", "R*", "K*", "B*", "P*", "  ", "♔ ", "♕ ", "♖ ", "♘ ", "♗ ", "♙ ", "♚ ", "♛ ", "♜ ", "♞ ", "♝ ", "♟︎ ", "  "};

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
    _castling_rights = CASTLE_KING_BLACK + CASTLE_KING_WHITE + CASTLE_QUEEN_BLACK + CASTLE_QUEEN_WHITE;
    _en_passant_square = -1;
    _fifty_move = 0;
    _material = 0;

    for (int i = 0; i < N_SQUARES; i++)
    {
        _square[i] = EMPTY;
    }

    _white_on_bottom = true;
}

void Board::setStartingPosition()
{
    this->clear();

    _square[E1] = WHITE_KING;
    _square[D1] = WHITE_QUEEN;
    _square[A1] = WHITE_ROOK;
    _square[H1] = WHITE_ROOK;
    _square[B1] = WHITE_KNIGHT;
    _square[G1] = WHITE_KNIGHT;
    _square[C1] = WHITE_BISHOP;
    _square[F1] = WHITE_BISHOP;
    _square[A2] = WHITE_PAWN;
    _square[B2] = WHITE_PAWN;
    _square[C2] = WHITE_PAWN;
    _square[D2] = WHITE_PAWN;
    _square[E2] = WHITE_PAWN;
    _square[F2] = WHITE_PAWN;
    _square[G2] = WHITE_PAWN;
    _square[H2] = WHITE_PAWN;
    _square[E8] = BLACK_KING;
    _square[D8] = BLACK_QUEEN;
    _square[A8] = BLACK_ROOK;
    _square[H8] = BLACK_ROOK;
    _square[B8] = BLACK_KNIGHT;
    _square[G8] = BLACK_KNIGHT;
    _square[C8] = BLACK_BISHOP;
    _square[F8] = BLACK_BISHOP;
    _square[A7] = BLACK_PAWN;
    _square[B7] = BLACK_PAWN;
    _square[C7] = BLACK_PAWN;
    _square[D7] = BLACK_PAWN;
    _square[E7] = BLACK_PAWN;
    _square[F7] = BLACK_PAWN;
    _square[G7] = BLACK_PAWN;
    _square[H7] = BLACK_PAWN;

    for (int i = 0; i < N_SQUARES; i++)
    {
        if (_square[i] == WHITE_KING)
        {
            _white_king = _white_king | Tables::SQUARE_BB[i];
        }
        else if (_square[i] == WHITE_QUEEN)
        {
            _white_queens = _white_queens | Tables::SQUARE_BB[i];
        }
        else if (_square[i] == WHITE_ROOK)
        {
            _white_rooks = _white_rooks | Tables::SQUARE_BB[i];
        }
        else if (_square[i] == WHITE_BISHOP)
        {
            _white_bishops = _white_bishops | Tables::SQUARE_BB[i];
        }
        else if (_square[i] == WHITE_KNIGHT)
        {
            _white_knights = _white_knights | Tables::SQUARE_BB[i];
        }
        else if (_square[i] == WHITE_PAWN)
        {
            _white_pawns = _white_pawns | Tables::SQUARE_BB[i];
        }
        else if (_square[i] == BLACK_KING)
        {
            _black_king = _black_king | Tables::SQUARE_BB[i];
        }
        else if (_square[i] == BLACK_QUEEN)
        {
            _black_queens = _black_queens | Tables::SQUARE_BB[i];
        }
        else if (_square[i] == BLACK_ROOK)
        {
            _black_rooks = _black_rooks | Tables::SQUARE_BB[i];
        }
        else if (_square[i] == BLACK_BISHOP)
        {
            _black_bishops = _black_bishops | Tables::SQUARE_BB[i];
        }
        else if (_square[i] == BLACK_KNIGHT)
        {
            _black_knights = _black_knights | Tables::SQUARE_BB[i];
        }
        else if (_square[i] == BLACK_PAWN)
        {
            _black_pawns = _black_pawns | Tables::SQUARE_BB[i];
        }
    }

    _white_pieces = _white_king | _white_queens | _white_rooks | _white_bishops | _white_knights | _white_pawns;
    _black_pieces = _black_king | _black_queens | _black_rooks | _black_bishops | _black_knights | _black_pawns;
    _occupied_squares = _white_pieces | _black_pieces;
}

void Board::setFromFen() // TODO: to be implemented
{
}

int Board::getMaterial() const
{
    return _material;
}
int Board::getCastlingRights() const
{
    return _castling_rights;
}

int Board::getEnPassantSquare() const
{
    return _en_passant_square;
}
int Board::getFiftyMove() const
{
    return _fifty_move;
}
int Board::getWhitePawnsCount() const
{
    return Utils::bitCount(_white_pawns);
}
int Board::getBlackPawnsCount() const
{
    return Utils::bitCount(_black_pawns);
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