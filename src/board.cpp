#include "board.hpp"
#include "bitboard.hpp"
#include "tables.hpp"
#include "utils.hpp"
#include <iostream>
#include <iomanip>

std::ostream &operator<<(std::ostream &os, const Board &board) // TODO: some improvements can be made to tis function
{
    // TODO: take it out of here
    static const char *PIECENAMES[13] = {"K ", "Q ", "R ", "K ", "B ", "P ", "K*", "Q*", "R*", "K*", "B*", "P*", "  "};

    int rank, file;
    os << std::endl;
    if (!board._view_rotated)
    {
        for (rank = 7; rank >= 0; rank--)
        {
            os << "    +---+---+---+---+---+---+---+---+" << std::endl;
            os << std::setw(3) << rank << " |";
            for (file = 0; file < 8; file++)
            {
                os << " " << PIECENAMES[board._square[utils::get_square_index(rank, file)]] << "|";
            }
            os << std::endl;
        }
        os << "    +---+---+---+---+---+---+---+---+" << std::endl;
        os << "      a   b   c   d   e   f   g   h" << std::endl
           << std::endl;
    }
    else
    {
        os << "      h   g   f   e   d   c   b   a" << std::endl;
        for (rank = 0; rank < 8; rank++)
        {
            os << "    +---+---+---+---+---+---+---+---+" << std::endl;
            os << "    |";
            for (file = 7; file >= 0; file--)
            {
                os << " " << PIECENAMES[board._square[utils::get_square_index(rank, file)]] << "|";
            }
            os << std::setw(3) << rank << std::endl;
        }
        os << "    +---+---+---+---+---+---+---+---+" << std::endl
           << std::endl;
    }
    return os;
}

Board::Board()
{
    this->init();
}

void Board::init()
{
    _view_rotated = false;

    for (int i = 0; i < 64; i++)
    {
        _square[i] = board::EMPTY;
    }

    _square[bb::E1] = board::WHITE_KING;
    _square[bb::D1] = board::WHITE_QUEEN;
    _square[bb::A1] = board::WHITE_ROOK;
    _square[bb::H1] = board::WHITE_ROOK;
    _square[bb::B1] = board::WHITE_KNIGHT;
    _square[bb::G1] = board::WHITE_KNIGHT;
    _square[bb::C1] = board::WHITE_BISHOP;
    _square[bb::F1] = board::WHITE_BISHOP;
    _square[bb::A2] = board::WHITE_PAWN;
    _square[bb::B2] = board::WHITE_PAWN;
    _square[bb::C2] = board::WHITE_PAWN;
    _square[bb::D2] = board::WHITE_PAWN;
    _square[bb::E2] = board::WHITE_PAWN;
    _square[bb::F2] = board::WHITE_PAWN;
    _square[bb::G2] = board::WHITE_PAWN;
    _square[bb::H2] = board::WHITE_PAWN;
    _square[bb::E8] = board::BLACK_KING;
    _square[bb::D8] = board::BLACK_QUEEN;
    _square[bb::A8] = board::BLACK_ROOK;
    _square[bb::H8] = board::BLACK_ROOK;
    _square[bb::B8] = board::BLACK_KNIGHT;
    _square[bb::G8] = board::BLACK_KNIGHT;
    _square[bb::C8] = board::BLACK_BISHOP;
    _square[bb::F8] = board::BLACK_BISHOP;
    _square[bb::A7] = board::BLACK_PAWN;
    _square[bb::B7] = board::BLACK_PAWN;
    _square[bb::C7] = board::BLACK_PAWN;
    _square[bb::D7] = board::BLACK_PAWN;
    _square[bb::E7] = board::BLACK_PAWN;
    _square[bb::F7] = board::BLACK_PAWN;
    _square[bb::G7] = board::BLACK_PAWN;
    _square[bb::H7] = board::BLACK_PAWN;

    // TODO: change function signature?
    this->initFromSquares(_square, true, 0, board::CANCASTLEOO + board::CANCASTLEOOO, board::CANCASTLEOO + board::CANCASTLEOOO, 0);
}

void Board::initFromSquares(int input[64], bool next, int fifty_move, int castle_white, int clastle_black, int ep_square)
{

    _white_king = utils::ZERO;
    _white_queens = utils::ZERO;
    _white_rooks = utils::ZERO;
    _white_bishops = utils::ZERO;
    _white_knights = utils::ZERO;
    _white_pawns = utils::ZERO;
    _black_king = utils::ZERO;
    _black_queens = utils::ZERO;
    _black_rooks = utils::ZERO;
    _black_bishops = utils::ZERO;
    _black_knights = utils::ZERO;
    _black_pawns = utils::ZERO;
    _white_pieces = utils::ZERO;
    _black_pieces = utils::ZERO;
    _occupied_squares = utils::ZERO;

    for (int i = 0; i < 64; i++)
    {
        _square[i] = input[i];

        if (_square[i] == board::WHITE_KING)
        {
            _white_king = _white_king | tables::SQUARE_BB[i];
        }
        else if (_square[i] == board::WHITE_QUEEN)
        {
            _white_queens = _white_queens | tables::SQUARE_BB[i];
        }
        else if (_square[i] == board::WHITE_ROOK)
        {
            _white_rooks = _white_rooks | tables::SQUARE_BB[i];
        }
        else if (_square[i] == board::WHITE_BISHOP)
        {
            _white_bishops = _white_bishops | tables::SQUARE_BB[i];
        }
        else if (_square[i] == board::WHITE_KNIGHT)
        {
            _white_knights = _white_knights | tables::SQUARE_BB[i];
        }
        else if (_square[i] == board::WHITE_PAWN)
        {
            _white_pawns = _white_pawns | tables::SQUARE_BB[i];
        }
        else if (_square[i] == board::BLACK_KING)
        {
            _black_king = _black_king | tables::SQUARE_BB[i];
        }
        else if (_square[i] == board::BLACK_QUEEN)
        {
            _black_queens = _black_queens | tables::SQUARE_BB[i];
        }
        else if (_square[i] == board::BLACK_ROOK)
        {
            _black_rooks = _black_rooks | tables::SQUARE_BB[i];
        }
        else if (_square[i] == board::BLACK_BISHOP)
        {
            _black_bishops = _black_bishops | tables::SQUARE_BB[i];
        }
        else if (_square[i] == board::BLACK_KNIGHT)
        {
            _black_knights = _black_knights | tables::SQUARE_BB[i];
        }
        else if (_square[i] == board::BLACK_PAWN)
        {
            _black_pawns = _black_pawns | tables::SQUARE_BB[i];
        }
    }

    _white_pieces = _white_king | _white_queens | _white_rooks | _white_bishops | _white_knights | _white_pawns;
    _black_pieces = _black_king | _black_queens | _black_rooks | _black_bishops | _black_knights | _black_pawns;
    _occupied_squares = _white_pieces | _black_pieces;

    _white_to_move = next;
    _castle_white = castle_white;
    _castle_black = clastle_black;
    _ep_square = (bb::Square)ep_square;
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