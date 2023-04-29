#pragma once

#include <codchess/move.hpp>

#include <vector>

namespace codbrain {
class PVTable {
  private:
    static const int MAX_DEPTH = 64;

    int _length[MAX_DEPTH]{};
    codchess::Move _table[MAX_DEPTH][MAX_DEPTH]{};

  public:
    codchess::Move get_pv_move(int depth) const;
    std::vector<codchess::Move> get_pv();
    std::vector<codchess::Move> get_pv_from_depth(int start_depth);

    void clear();
    void set_length(int depth);
    void add(codchess::Move const move, int depth);
    void add_pv_from_depth(std::vector<codchess::Move> moves,
                           int starting_depth);
};
}   // namespace codbrain