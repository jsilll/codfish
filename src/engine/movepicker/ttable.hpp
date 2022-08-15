#pragma once

#include <engine/bitboard.hpp>

class TTable
{
private:
    struct TTEntry
    {
        u64 hash_key;
        int depth;
        int flag;
        int score;
        // TODO: List Of Moves
    };

    static const int TABLE_SIZE = 0x400000;

    TTEntry *_table = new TTEntry[TABLE_SIZE];

public:
    static const int HASH_FLAG_SCORE = 0;
    static const int HASH_FLAG_ALPHA = 1;
    static const int HASH_FLAG_BETA = 2;

    struct TTOutput
    {
        bool found;
        int score;
        // TODO: List Of Moves
    };

    ~TTable();

    void clear();
    void set_entry(u64 hash_key, int depth, int flag, int score);
    TTOutput read_hash(u64 hash_key, int alpha, int beta, int depth);
};