#include "board.hpp"
#include "bitboard.hpp"
#include "tables.hpp"
#include "utils.hpp"
#include <iostream>
#include <iomanip>

std::ostream &operator<<(std::ostream &os, const board::Board &board)
{
    return os;
}

void board::Board::init()
{
    _view_rotated = false;

    for (int i = 0; i < 64; i++)
    {
        _square[i] = EMPTY;
    }

    _square[bb::E1] = WHITE_KING;
    _square[bb::D1] = WHITE_QUEEN;
    _square[bb::A1] = WHITE_ROOK;
    _square[bb::H1] = WHITE_ROOK;
    _square[bb::B1] = WHITE_KNIGHT;
    _square[bb::G1] = WHITE_KNIGHT;
    _square[bb::C1] = WHITE_BISHOP;
    _square[bb::F1] = WHITE_BISHOP;
    _square[bb::A2] = WHITE_PAWN;
    _square[bb::B2] = WHITE_PAWN;
    _square[bb::C2] = WHITE_PAWN;
    _square[bb::D2] = WHITE_PAWN;
    _square[bb::E2] = WHITE_PAWN;
    _square[bb::F2] = WHITE_PAWN;
    _square[bb::G2] = WHITE_PAWN;
    _square[bb::H2] = WHITE_PAWN;
    _square[bb::E8] = BLACK_KING;
    _square[bb::D8] = BLACK_QUEEN;
    _square[bb::A8] = BLACK_ROOK;
    _square[bb::H8] = BLACK_ROOK;
    _square[bb::B8] = BLACK_KNIGHT;
    _square[bb::G8] = BLACK_KNIGHT;
    _square[bb::C8] = BLACK_BISHOP;
    _square[bb::F8] = BLACK_BISHOP;
    _square[bb::A7] = BLACK_PAWN;
    _square[bb::B7] = BLACK_PAWN;
    _square[bb::C7] = BLACK_PAWN;
    _square[bb::D7] = BLACK_PAWN;
    _square[bb::E7] = BLACK_PAWN;
    _square[bb::F7] = BLACK_PAWN;
    _square[bb::G7] = BLACK_PAWN;
    _square[bb::H7] = BLACK_PAWN;

    this->initFromSquares(_square, true, 0, CANCASTLEOO + CANCASTLEOOO, CANCASTLEOO + CANCASTLEOOO, 0);
}

void board::Board::initFromSquares(int input[64], unsigned char next, int fiftyM, int castleW, int castleB, int epSq)
{
    // sets up the board variables according to the information found in
    // the input[64] array
    // All board & game initializations are done through this function (including readfen and setup).

    int i;

    // bitboards
    _white_king = 0;
    _white_queens = 0;
    _white_rooks = 0;
    _white_bishops = 0;
    _white_knights = 0;
    _white_pawns = 0;
    _black_king = 0;
    _black_queens = 0;
    _black_rooks = 0;
    _black_bishops = 0;
    _black_knights = 0;
    _black_pawns = 0;
    _white_pieces = 0;
    _black_pieces = 0;
    _occupied_squares = 0;

    // populate the 12 bitboard:
    for (i = 0; i < 64; i++)
    {
        _square[i] = input[i];
        if (_square[i] == WHITE_KING)
            _white_king = _white_king | tables::SQUARE_BB[i];
        if (_square[i] == WHITE_QUEEN)
            _white_queens = _white_queens | tables::SQUARE_BB[i];
        if (_square[i] == WHITE_ROOK)
            _white_rooks = _white_rooks | tables::SQUARE_BB[i];
        if (_square[i] == WHITE_BISHOP)
            _white_bishops = _white_bishops | tables::SQUARE_BB[i];
        if (_square[i] == WHITE_KNIGHT)
            _white_knights = _white_knights | tables::SQUARE_BB[i];
        if (_square[i] == WHITE_PAWN)
            _white_pawns = _white_pawns | tables::SQUARE_BB[i];
        if (_square[i] == BLACK_KING)
            _black_king = _black_king | tables::SQUARE_BB[i];
        if (_square[i] == BLACK_QUEEN)
            _black_queens = _black_queens | tables::SQUARE_BB[i];
        if (_square[i] == BLACK_ROOK)
            _black_rooks = _black_rooks | tables::SQUARE_BB[i];
        if (_square[i] == BLACK_BISHOP)
            _black_bishops = _black_bishops | tables::SQUARE_BB[i];
        if (_square[i] == BLACK_KNIGHT)
            _black_knights = _black_knights | tables::SQUARE_BB[i];
        if (_square[i] == BLACK_PAWN)
            _black_pawns = _black_pawns | tables::SQUARE_BB[i];
    }

    _white_pieces = _white_king | _white_queens | _white_rooks | _white_bishops | _white_knights | _white_pawns;
    _black_pieces = _black_king | _black_queens | _black_rooks | _black_bishops | _black_knights | _black_pawns;
    _occupied_squares = _white_pieces | _black_pieces;

    _next_move = next;
    _castle_white = castleW;
    _castle_black = castleB;
    _ep_square = (bb::Square)epSq;
    _fifty_move = fiftyM;

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

void board::Board::display()
{
    // TODO take it out of here
    static const char *PIECENAMES[16] = {"  ", "P ", "K ", "N ", "  ", "B ", "R ", "Q ",
                                         "  ", "P*", "K*", "N*", "  ", "B*", "R*", "Q*"};

    int rank, file;

    std::cout << std::endl;
    {
        if (!_view_rotated)
        {
            for (rank = 7; rank >= 0; rank--)
            {
                std::cout << "    +---+---+---+---+---+---+---+---+" << std::endl;
                std::cout << std::setw(3) << rank << " |";
                for (file = 0; file < 8; file++)
                {
                    std::cout << " " << PIECENAMES[_square[utils::get_square_index(file, rank)]] << "|";
                }
                std::cout << std::endl;
            }
            std::cout << "    +---+---+---+---+---+---+---+---+" << std::endl;
            std::cout << "      a   b   c   d   e   f   g   h" << std::endl
                      << std::endl;
        }
        else
        {
            std::cout << "      h   g   f   e   d   c   b   a" << std::endl;
            for (rank = 0; rank < 8; rank++)
            {
                std::cout << "    +---+---+---+---+---+---+---+---+" << std::endl;
                std::cout << "    |";
                for (file = 7; file >= 0; file--)
                {
                    std::cout << " " << PIECENAMES[_square[utils::get_square_index(file, rank)]] << "|";
                }
                std::cout << std::setw(3) << rank << std::endl;
            }
            std::cout << "    +---+---+---+---+---+---+---+---+" << std::endl
                      << std::endl;
        }
    }
}