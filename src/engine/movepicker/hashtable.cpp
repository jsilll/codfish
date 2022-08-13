#include <engine/movepicker/hashtable.hpp>

#include <cstring>

void TranspositionTable::clear()
{
    memset(_table, 0, sizeof(_table));
}

void TranspositionTable::set_entry(u64 hash_key, int depth, int flag, int score)
{
    int entry = hash_key % HASH_SIZE;
    _table[entry].depth = depth;
    _table[entry].flag = flag;
    _table[entry].score = score;
    // TTEntry *hash_entry = &_table[hash_key % HASH_SIZE];
    // hash_entry->hash_key = hash_key;
    // hash_entry->depth = depth;
    // hash_entry->flag = flag;
    // hash_entry->score = score;
}

TranspositionTable::TTOutput TranspositionTable::read_hash(u64 hash_key, int alpha, int beta, int depth)
{
    TTEntry *hash_entry = &_table[hash_key % HASH_SIZE];
    TTOutput search_result = {false, 0};

    if ((hash_entry->hash_key == hash_key) && (hash_entry->depth >= depth))
    {
        search_result.found = true;
        if (hash_entry->flag == HASH_FLAG_SCORE)
        {
            search_result.score = hash_entry->score;
        }

        if ((hash_entry->flag == HASH_FLAG_ALPHA) && (hash_entry->score <= alpha))
        {
            search_result.score = alpha;
        }

        if ((hash_entry->flag == HASH_FLAG_BETA) && (hash_entry->score > beta))
        {
            search_result.score = beta;
        }
    }

    return search_result;
}