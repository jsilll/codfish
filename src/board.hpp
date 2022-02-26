#pragma once

#include "defs.hpp"

#include <iostream>

constexpr int CASTLE_KING_WHITE = 1;
constexpr int CASTLE_QUEEN_WHITE = 2;
constexpr int CASTLE_KING_BLACK = 4;
constexpr int CASTLE_QUEEN_BLACK = 8;

struct Piece
{
    PieceType type;
    PieceColor color;
};

class Board
{
private:
    U64 _pieces[BOTH][EMPTY];
    U64 _occupancies[BOTH + 1];
    int _to_move;
    int _castling_rights;
    int _en_passant_square;
    int _half_move_clock;
    int _full_move_number;

    bool _white_on_bottom;
    Piece _square[N_SQUARES];

public:
    // board.cpp
    Board();
    void clear();
    void updateBBFromSquares();
    void setStartingPosition();
    void setFromFen(std::string piece_placements,
                    std::string active_color,
                    std::string castling_rights,
                    std::string en_passant,
                    std::string halfmove_clock,
                    std::string fullmove_number);
    int switchSideToMove();
    bool rotate();

    int getColorToMove() const;
    int getCastlingRights() const;
    int getEnPassantSquare() const;
    int getHalfMoveClock() const;
    int getFullMoveNumber() const;
    U64 getOccupiedSquares() const;
    std::string getFen() const;

    bool isSquareAttacked(const int sq) const;
    void getLegalMoves() const;

    void print(bool ascii = false) const;
};
