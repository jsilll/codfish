#include <codbrain/ttable.hpp>

#include <cstring>
#include <iostream>

#include <codchess/bitboard.hpp>

using namespace codchess;
using namespace bitboard;

namespace codbrain {
TTable::~TTable() { delete[] _table; }

void
TTable::clear() {
    for (int p = 0; p < TABLE_SIZE; p++) {
        _table[p] = {0ULL, 0, 0, 0, {}};
    }
}

void
TTable::set_entry(std::uint64_t hash_key, int depth, int flag, int score,
                  std::vector<Move> moves) {
    int entry = hash_key % TABLE_SIZE;
    _table[entry].depth = depth;
    _table[entry].flag = flag;
    _table[entry].score = score;
    _table[entry].moves = moves;
}

TTable::TTOutput
TTable::read_hash(Bitboard hash_key, int alpha, int beta, int depth) {
    TTOutput search_result = {false, 0, {}};
    TTEntry hash_entry = _table[hash_key % TABLE_SIZE];
    if ((hash_entry.hash_key == hash_key) && (hash_entry.depth >= depth)) {
        search_result.found = true;
        search_result.moves = hash_entry.moves;
        if (hash_entry.flag == HASH_FLAG_SCORE) {
            search_result.score = hash_entry.score;
            search_result.moves = hash_entry.moves;
        }

        if ((hash_entry.flag == HASH_FLAG_ALPHA) &&
            (hash_entry.score <= alpha)) {
            search_result.score = alpha;
            search_result.moves = hash_entry.moves;
        }

        if ((hash_entry.flag == HASH_FLAG_BETA) && (hash_entry.score > beta)) {
            search_result.score = beta;
            search_result.moves = hash_entry.moves;
        }
    }

    return search_result;
}
}   // namespace codbrain