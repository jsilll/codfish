#pragma once

#include <engine/movegen/move.hpp>

#include <vector>

class PVTable
{
private:
    static const int MAX_DEPTH = 64;

    int _length[MAX_DEPTH]{};
    Move _table[MAX_DEPTH][MAX_DEPTH]{};

public:
    Move getPVMove(int depth);
    std::vector<Move> getPV();

    void clear();
    void setLength(int depth);
    void add(Move const &move, int depth);
};