#pragma once

#include "defs.hpp"
#include <iostream>

class Board
{
private:
    U64 _white_king, _white_queens, _white_rooks, _white_bishops, _white_knights, _white_pawns;
    U64 _black_king, _black_queens, _black_rooks, _black_bishops, _black_knights, _black_pawns;
    U64 _white_pieces, _black_pieces, _occupied_squares;

    bool _white_to_move;
    int _castle_white;
    int _castle_black;
    int _en_passant_square;
    int _fifty_move;
    int _material;

    int _square[N_SQUARES];
    bool _white_on_bottom;

public:
    Board();

    void reset();
    void initFromSquares(int input[N_SQUARES], bool next, int fifty_move, int castle_white, int clastle_black, int ep_square);
    void initFromFen(const char *fen, const char *fencolor, const char *fencastling, const char *fenenpassant, const char *char_fenhalfmoveclock, const char *char_fenfullmovenumber);

    int getMaterial() const;
    int getCastleWhite() const;
    int getCastleBlack() const;
    int getEnPassantSquare() const;
    int getFiftyMove() const;
    int getWhitePawnsCount() const;
    int getBlackPawnsCount() const;

    bool switchSideToMove();

    bool rotate();
    void print(bool ascii);
};
