#pragma once

#include "codlib/bitboard.hpp"

class HistoryTable
{
private:
    static const int MAX_HISTORY_SIZE = 128;

    int _index = 0;
    u64 *_hist_table = new u64[MAX_HISTORY_SIZE];

public:
    ~HistoryTable();

    bool is_repetition(u64 key) const;

    void push(u64 key);
    void pop();

    void clear();
};