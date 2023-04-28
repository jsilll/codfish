#include <codbrain/histtable.hpp>

namespace codbrain {
HistoryTable::~HistoryTable() { delete[] _hist_table; }

bool
HistoryTable::is_repetition(std::uint64_t key) const {
    for (int index = 0; index < _index; index++) {
        if (_hist_table[index] == key) {
            return true;
        }
    }

    return false;
}

void
HistoryTable::push(std::uint64_t key) {
    _hist_table[_index++] = key;
}

void
HistoryTable::pop() {
    --_index;
}

void
HistoryTable::clear() {
    _index = 0;
}
}   // namespace codbrain