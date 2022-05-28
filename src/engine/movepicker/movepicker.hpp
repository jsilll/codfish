#pragma once

#include "../defs.hpp"

class Board;
class Move;

class MovePicker
{
private:
    Board &_board;
    int _depth;
    unsigned long long _nodes;

public:
    struct SearchResult
    {
        int score;
        int best_move_encoded;
        unsigned long long nodes;
    };

public:
    MovePicker(Board &board) : _board(board), _depth(1), _nodes(0) {}

    int getDepth() const;

    void setDepth(int depth);

    SearchResult findBestMove();

private:
    int search(int alpha, int beta, int depth, const Board &board);
    int quiescence(int alpha, int beta, int depth, const Board &board);
};