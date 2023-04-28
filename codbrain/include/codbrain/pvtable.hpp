#pragma once

#include <codchess/move.hpp>

#include <vector>

using namespace codchess;

namespace codbrain {
class PVTable {
  private:
    static const int MAX_DEPTH = 64;

    int _length[MAX_DEPTH]{};
    Move _table[MAX_DEPTH][MAX_DEPTH]{};

  public:
    Move get_pv_move(int depth);
    std::vector<Move> get_pv();
    std::vector<Move> get_pv_from_depth(int start_depth);

    void clear();
    void set_length(int depth);
    void add(Move const move, int depth);
    void add_pv_from_depth(std::vector<Move> moves, int starting_depth);
};
}   // namespace codbrain