#include <engine/movepicker/histtable.hpp>

HistoryTable::~HistoryTable()
{
    delete[] _hist_table;
}

bool HistoryTable::is_repetition(u64 key) const
{
    for (int index = 0; index < _index; index++)
    {
        if (_hist_table[index] == key)
        {
            return true;
        }
    }

    return false;
}

void HistoryTable::push(u64 key)
{
    _hist_table[_index++] = key;
}

void HistoryTable::pop()
{
    --_index;
}

void HistoryTable::clear()
{
    _index = 0;
}
