#pragma once

#include <cstdint>

namespace codbrain {
class HistoryTable {
  private:
    static const int MAX_HISTORY_SIZE = 128;

    int _index = 0;
    std::uint64_t *_hist_table = new std::uint64_t[MAX_HISTORY_SIZE];

  public:
    ~HistoryTable();

    bool is_repetition(std::uint64_t key) const;

    void push(std::uint64_t key);
    void pop();

    void clear();
};
}   // namespace codbrain