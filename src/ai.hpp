#pragma once

#include "defs.hpp"

class Board;
class Move;

class AI
{
private:
    Board &_board;

public:
    AI(Board &board) : _board(board) {}

    Move find_best_move();

private:
    int search(int alpha, int beta, int depth, Board &board);
};