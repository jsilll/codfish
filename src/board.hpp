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
    int _castling_rights;
    int _en_passant_square;
    int _fifty_move;
    int _material;

    int _square[N_SQUARES];
    bool _white_on_bottom;

public:
    Board();

    void clear();

    void setStartingPosition();
    void setFromFen();
    bool switchSideToMove();

    int getMaterial() const;
    int getCastlingRights() const;
    int getEnPassantSquare() const;
    int getFiftyMove() const;
    int getWhitePawnsCount() const;
    int getBlackPawnsCount() const;

    bool isWhiteToMove() const;

    bool rotate();
    void print(bool ascii);
};
