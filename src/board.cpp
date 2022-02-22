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
                std::cout << " " << PIECE_NAMES[_square[utils::getSquare(rank, file)] + offset] << "|";
            }
            std::cout << std::setw(3) << rank << "\n";
        }
        std::cout << "    +---+---+---+---+---+---+---+---+\n";
    }
    else
    {
        for (int rank = 7; rank >= 0; rank--)
        {
            std::cout << "    +---+---+---+---+---+---+---+---+\n"
                      << std::setw(3) << rank << " |";

            for (int file = 0; file < 8; file++)
            {
                std::cout << " " << PIECE_NAMES[_square[utils::getSquare(rank, file)] + offset] << "|";
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
    this->reset();
}

void Board::reset()
{
    _white_on_bottom = true;

    for (int i = 0; i < N_SQUARES; i++)
    {
        _square[i] = EMPTY;
    }

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

    this->initFromSquares(_square, true, 0, CASTLE_KING_BLACK + CASTLE_KING_WHITE + CASTLE_QUEEN_BLACK + CASTLE_QUEEN_WHITE, 0);
}

void Board::initFromSquares(int input[N_SQUARES], bool next, int fifty_move, int castling_rights, int en_passant_square)
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
        _square[i] = input[i];

        if (_square[i] == WHITE_KING)
        {
            _white_king = _white_king | tables::SQUARE_BB[i];
        }
        else if (_square[i] == WHITE_QUEEN)
        {
            _white_queens = _white_queens | tables::SQUARE_BB[i];
        }
        else if (_square[i] == WHITE_ROOK)
        {
            _white_rooks = _white_rooks | tables::SQUARE_BB[i];
        }
        else if (_square[i] == WHITE_BISHOP)
        {
            _white_bishops = _white_bishops | tables::SQUARE_BB[i];
        }
        else if (_square[i] == WHITE_KNIGHT)
        {
            _white_knights = _white_knights | tables::SQUARE_BB[i];
        }
        else if (_square[i] == WHITE_PAWN)
        {
            _white_pawns = _white_pawns | tables::SQUARE_BB[i];
        }
        else if (_square[i] == BLACK_KING)
        {
            _black_king = _black_king | tables::SQUARE_BB[i];
        }
        else if (_square[i] == BLACK_QUEEN)
        {
            _black_queens = _black_queens | tables::SQUARE_BB[i];
        }
        else if (_square[i] == BLACK_ROOK)
        {
            _black_rooks = _black_rooks | tables::SQUARE_BB[i];
        }
        else if (_square[i] == BLACK_BISHOP)
        {
            _black_bishops = _black_bishops | tables::SQUARE_BB[i];
        }
        else if (_square[i] == BLACK_KNIGHT)
        {
            _black_knights = _black_knights | tables::SQUARE_BB[i];
        }
        else if (_square[i] == BLACK_PAWN)
        {
            _black_pawns = _black_pawns | tables::SQUARE_BB[i];
        }
    }

    _white_pieces = _white_king | _white_queens | _white_rooks | _white_bishops | _white_knights | _white_pawns;
    _black_pieces = _black_king | _black_queens | _black_rooks | _black_bishops | _black_knights | _black_pawns;
    _occupied_squares = _white_pieces | _black_pieces;

    _white_to_move = next;
    _castling_rights = castling_rights;
    _en_passant_square = en_passant_square;
    _fifty_move = fifty_move;

    _material = 0;
    // _material = bitCnt(whitePawns) * PAWN_VALUE +
    //             bitCnt(whiteKnights) * KNIGHT_VALUE +
    //             bitCnt(whiteBishops) * BISHOP_VALUE +
    //             bitCnt(whiteRooks) * ROOK_VALUE +
    //             bitCnt(whiteQueens) * QUEEN_VALUE;
    // _material -= (bitCnt(blackPawns) * PAWN_VALUE +
    //               bitCnt(blackKnights) * KNIGHT_VALUE +
    //               bitCnt(blackBishops) * BISHOP_VALUE +
    //               bitCnt(blackRooks) * ROOK_VALUE +
    //               bitCnt(blackQueens) * QUEEN_VALUE);
}

// TODO: debug and clean this funciton's code
void Board::initFromFen(const char *fen, const char *fencolor, const char *fencastling, const char *fenenpassant, const char *char_fenhalfmoveclock, const char *char_fenfullmovenumber)
{
    int i, file, rank, counter, piece;
    int whiteCastle, blackCastle, epsq;
    bool white_to_move;

    int fenhalfmoveclock, fenfullmovenumber;
    scanf(char_fenhalfmoveclock, "%d", &fenhalfmoveclock);   // int, used for the fifty move draw rule
    scanf(char_fenfullmovenumber, "%d", &fenfullmovenumber); // int. start with 1, It is incremented after move by Black

    piece = 0;
    for (i = 0; i < 64; i++)
    {
        _square[i] = EMPTY;
    }

    file = 1;
    rank = 8;
    i = 0;
    counter = 0;
    while ((counter < 64) && (fen[i] != '\0'))
    {
        // '1'  through '8':
        if (((int)fen[i] > 48) && ((int)fen[i] < 57))
        {
            file += (int)fen[i] - 48;
            counter += (int)fen[i] - 48;
        }
        else
        //  other characters:
        {
            switch (fen[i])
            {
            case '/':
                rank--;
                file = 1;
                break;

            case 'P':
                _square[utils::getSquare(rank, file)] = WHITE_PAWN;
                file += 1;
                counter += 1;
                break;

            case 'N':
                _square[utils::getSquare(rank, file)] = WHITE_KNIGHT;
                file += 1;
                counter += 1;
                break;

            case 'B':
                _square[utils::getSquare(rank, file)] = WHITE_BISHOP;
                file += 1;
                counter += 1;
                break;

            case 'R':
                _square[utils::getSquare(rank, file)] = WHITE_ROOK;
                file += 1;
                counter += 1;
                break;

            case 'Q':
                _square[utils::getSquare(rank, file)] = WHITE_QUEEN;
                file += 1;
                counter += 1;
                break;

            case 'K':
                _square[utils::getSquare(rank, file)] = WHITE_KING;
                file += 1;
                counter += 1;
                break;

            case 'p':
                _square[utils::getSquare(rank, file)] = BLACK_PAWN;
                file += 1;
                counter += 1;
                break;

            case 'n':
                _square[utils::getSquare(rank, file)] = BLACK_KNIGHT;
                file += 1;
                counter += 1;
                break;

            case 'b':
                _square[utils::getSquare(rank, file)] = BLACK_BISHOP;
                file += 1;
                counter += 1;
                break;

            case 'r':
                _square[utils::getSquare(rank, file)] = BLACK_ROOK;
                file += 1;
                counter += 1;
                break;

            case 'q':
                _square[utils::getSquare(rank, file)] = BLACK_QUEEN;
                file += 1;
                counter += 1;
                break;

            case 'k':
                _square[utils::getSquare(rank, file)] = BLACK_KING;
                file += 1;
                counter += 1;
                break;

            default:
                break;
            }
        }
        i++;
    }
    white_to_move = true;
    if (fencolor[0] == 'b')
        white_to_move = false;

    int castling_rights = 0;
    if (strstr(fencastling, "K"))
        castling_rights += CASTLE_KING_WHITE;
    if (strstr(fencastling, "Q"))
        castling_rights += CASTLE_QUEEN_WHITE;
    if (strstr(fencastling, "k"))
        castling_rights += CASTLE_KING_BLACK;
    if (strstr(fencastling, "q"))
        castling_rights += CASTLE_QUEEN_BLACK;
    if (strstr(fenenpassant, "-"))
    {
        epsq = 0;
    }
    else
    {
        // translate a square coordinate (as string) to int (eg 'e3' to 20):
        epsq = ((int)fenenpassant[0] - 96) + 8 * ((int)fenenpassant[1] - 48) - 9;
    }

    initFromSquares(_square, white_to_move, fenhalfmoveclock, castling_rights, epsq);
}

int Board::getMaterial() const
{
    return _material;
}
int Board::getCastleWhite() const
{
    static const int mask = CASTLE_KING_WHITE + CASTLE_QUEEN_WHITE;
    return _castling_rights & mask; // TODO: maybe improve performance of this
}
int Board::getCastleBlack() const
{
    static const int mask = CASTLE_KING_BLACK + CASTLE_QUEEN_BLACK;
    return _castling_rights & mask; // TODO: maybe improve performance of this
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
    return utils::bitCount(_white_pawns);
}
int Board::getBlackPawnsCount() const
{
    return utils::bitCount(_black_pawns);
}

bool Board::switchSideToMove()
{
    return _white_to_move = !_white_to_move;
}

bool Board::rotate()
{
    return _white_on_bottom = !_white_on_bottom;
}