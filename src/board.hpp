#pragma once

#include "bitboard.hpp"
#include <iostream>

namespace board
{

    enum CastleStatus
    {
        CANCASTLEOO,
        CANCASTLEOOO
    };

    enum Piece
    {
        WHITE_KING,
        WHITE_QUEEN,
        WHITE_ROOK,
        WHITE_KNIGHT,
        WHITE_BISHOP,
        WHITE_PAWN,
        BLACK_KING,
        BLACK_QUEEN,
        BLACK_ROOK,
        BLACK_KNIGHT,
        BLACK_BISHOP,
        BLACK_PAWN,
        EMPTY
    };

} // namespace board

class Board
{
public:
    U64 _white_king, _white_queens, _white_rooks, _white_bishops, _white_knights, _white_pawns;
    U64 _black_king, _black_queens, _black_rooks, _black_bishops, _black_knights, _black_pawns;
    U64 _white_pieces, _black_pieces, _occupied_squares;

    bool _white_to_move;   // WHITE_MOVE or BLACK_MOVE
    int _castle_white;     // White's castle status, CANCASTLEOO = 1, CANCASTLEOOO = 2
    int _castle_black;     // Black's castle status, CANCASTLEOO = 1, CANCASTLEOOO = 2
    bb::Square _ep_square; // En-passant target square after double pawn move
    int _fifty_move;       // Moves since the last pawn move or capture

    int _material;      // incrementally updated, total material on board,
                        // in centipawns, from white’s side of view
    int _square[64];    // incrementally updated, this array is usefull if we want to
                        // probe what kind of piece is on a particular square.
    bool _view_rotated; // only used for displaying the board. TRUE or FALSE.

    Board();

    void init();
    void initFromSquares(int input[64], bool next, int fifty_move, int castle_white, int clastle_black, int ep_square);
    void initFromFen(char fen[], char fencolor[], char fencastling[], char fenenpassant[], int fenhalfmoveclock, int fenfullmovenumber);

    friend std::ostream &operator<<(std::ostream &os, const Board &rhs);
};
