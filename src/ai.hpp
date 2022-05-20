#pragma once

#include "defs.hpp"

class Board;
class Move;

class AI
{
private:
    int _depth;
    Board &_board;

public:
    struct SearchResult
    {
        int score;
        int bestmove_encoded;
        unsigned long long nodes;
    };

public:
    AI(Board &board) : _board(board) {}

    void setDepth(int depth);

    int getDepth() const;
    SearchResult find_best_move() const;

private:
    int search(int alpha, int beta, int depth, Board &board) const;
};