#pragma once

#include <engine/constants.hpp>

#define hash_flag_score 0
#define hash_flag_alpha 1
#define hash_flag_beta 2

class TranspositionTable
{
private:
    struct TTEntry // transposition table entry
    {
        u64 hash_key;
        int depth;
        int flag;
        int score;
        // TODO: list of moves
    };

    static const int HASH_SIZE = 0x400000;

    TTEntry _table[HASH_SIZE]{};

public:
    struct TTOutput // transposition table entry
    {
        bool found;
        int score;
        // TODO: list of moves
    };

    void clear();
    void set_entry(u64 hash_key, int depth, int flag, int score);
    TTOutput read_hash(u64 hash_key, int alpha, int beta, int depth);
};