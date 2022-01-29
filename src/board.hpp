#pragma once

#include "defs.hpp"
#include <iostream>

class Board
{
public:
    U64 _white_king, _white_queens, _white_rooks, _white_bishops, _white_knights, _white_pawns;
    U64 _black_king, _black_queens, _black_rooks, _black_bishops, _black_knights, _black_pawns;
    U64 _white_pieces, _black_pieces, _occupied_squares;

    bool _white_to_move; // WHITE_MOVE or BLACK_MOVE
    int _castle_white;   // White's castle status, CANCASTLEOO = 1, CANCASTLEOOO = 2
    int _castle_black;   // Black's castle status, CANCASTLEOO = 1, CANCASTLEOOO = 2
    int _ep_square;      // En-passant target square after double pawn move
    int _fifty_move;     // Moves since the last pawn move or capture

    int _material;          // incrementally updated, total material on board,
                            // in centipawns, from white’s side of view
    int _square[N_SQUARES]; // incrementally updated, this array is usefull if we want to
                            // probe what kind of piece is on a particular square.
    bool _view_rotated;     // only used for displaying the board. TRUE or FALSE.

    Board();

    void init();
    void initFromSquares(int input[N_SQUARES], bool next, int fifty_move, int castle_white, int clastle_black, int ep_square);
    void initFromFen(const char *fen, const char *fencolor, const char *fencastling, const char *fenenpassant, const char *char_fenhalfmoveclock, const char *char_fenfullmovenumber);

    friend std::ostream &operator<<(std::ostream &os, const Board &rhs);
};
