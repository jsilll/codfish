#pragma once

#include "defs.hpp"

class Board;
class Move;

class AI
{
private:
    Board &_board;

public:
    struct SearchResult
    {
        int score;
        int bestmove_encoded;
        unsigned long long nodes; // TODO
    };

public:
    AI(Board &board) : _board(board) {}

    SearchResult find_best_move(int depth);

private:
    int search(int alpha, int beta, int depth, Board &board);
};