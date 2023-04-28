#pragma once

#include <vector>

#include <codchess/move.hpp>

namespace codbrain {
class TTable {
  private:
    struct TTEntry {
        std::uint64_t hash_key;
        int depth;
        int flag;
        int score;
        std::vector<codchess::Move> moves;
    };

    static const int TABLE_SIZE = 0x400000;

    TTEntry *_table = new TTEntry[TABLE_SIZE];

  public:
    static const int HASH_FLAG_SCORE = 0;
    static const int HASH_FLAG_ALPHA = 1;
    static const int HASH_FLAG_BETA = 2;

    struct TTOutput {
        bool found;
        int score;
        std::vector<codchess::Move> moves;
    };

    ~TTable();

    void clear();
    void set_entry(std::uint64_t hash_key, int depth, int flag, int score,
                   std::vector<codchess::Move> moves);
    TTOutput read_hash(std::uint64_t hash_key, int alpha, int beta, int depth);
};
}   // namespace codbrain