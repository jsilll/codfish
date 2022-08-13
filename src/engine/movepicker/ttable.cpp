#include <engine/movepicker/ttable.hpp>

#include <cstring>
#include <iostream>

TTable::~TTable()
{
    delete[] _table;
}

void TTable::clear()
{
    memset(_table, 0, TABLE_SIZE);
}

void TTable::set_entry([[maybe_unused]] u64 hash_key, int depth, int flag, int score)
{
    int entry = hash_key % TABLE_SIZE;
    _table[entry].depth = depth;
    _table[entry].flag = flag;
    _table[entry].score = score;
}

TTable::TTOutput TTable::read_hash(u64 hash_key, int alpha, int beta, int depth)
{
    TTOutput search_result = {false, 0};
    TTEntry hash_entry = _table[hash_key % TABLE_SIZE];
    if ((hash_entry.hash_key == hash_key) && (hash_entry.depth >= depth))
    {
        search_result.found = true;
        if (hash_entry.flag == HASH_FLAG_SCORE)
        {
            search_result.score = hash_entry.score;
        }

        if ((hash_entry.flag == HASH_FLAG_ALPHA) && (hash_entry.score <= alpha))
        {
            search_result.score = alpha;
        }

        if ((hash_entry.flag == HASH_FLAG_BETA) && (hash_entry.score > beta))
        {
            search_result.score = beta;
        }
    }

    return search_result;
}