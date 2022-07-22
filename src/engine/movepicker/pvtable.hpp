#pragma once

#include <engine/move.hpp>

#include <vector>

class PVTable
{
private:
    static const int MAX_DEPTH = 64;

    int _length[MAX_DEPTH]{};
    Move _table[MAX_DEPTH][MAX_DEPTH]{};

public:
    Move get_pv_move(int depth);
    std::vector<Move> get_pV();

    void clear();
    void set_length(int depth);
    void add(Move const move, int depth);
};