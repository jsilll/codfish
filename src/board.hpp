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
    int _half_move_clock;
    int _full_move_number;
    int _material;

    int _square[N_SQUARES];
    bool _white_on_bottom;

public:
    Board();
    void clear();
    void setStartingPosition();
    void updateBBFromSquares();
    void setFromFen(std::string piece_placements,
                    std::string active_color,
                    std::string castling_rights,
                    std::string en_passant,
                    std::string halfmove_clock,
                    std::string fullmove_number);
    bool switchSideToMove();
    bool rotate();
    void print(bool ascii);

    int getMaterial() const;
    int getCastlingRights() const;
    int getEnPassantSquare() const;
    int getHalfMoveClock() const;
    int getFullMoveNumber() const;
    int getWhitePawnsCount() const;
    int getBlackPawnsCount() const;
    bool isWhiteToMove() const;
    std::string toFen() const;
};
