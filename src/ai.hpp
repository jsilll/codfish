#pragma once

#include "defs.hpp"

class Board;
class Move;

class AI
{
public:
    AI(Board &board) : board(board) {}

    Move find_best_move();

private:
    Board &board;

    int search(int alpha, int beta, int level);
};