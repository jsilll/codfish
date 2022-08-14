#include <engine/movepicker/pvtable.hpp>

#include <engine/move.hpp>

#include <cstring>

Move PVTable::get_pv_move(int depth)
{
    return _table[depth][depth];
}

std::vector<Move> PVTable::get_pv()
{
    std::vector<Move> res;
    for (int i = 0; i < _length[0]; i++)
    {
        res.push_back(_table[0][i]);
    }

    return res;
}

void PVTable::add(Move const move, int depth)
{
    // Write Principal Variation Move
    _table[depth][depth] = move;

    // Copy moves from deeper depth into current depths line
    memcpy(&_table[depth][depth + 1], &_table[depth + 1][depth + 1], (unsigned long)_length[depth + 1] * sizeof(int));

    // Adjust Principal Variation Length
    _length[depth] = _length[depth + 1];
}

void PVTable::clear()
{
    memset(_table, 0, sizeof(_table));
}

void PVTable::set_length(int depth)
{
    _length[depth] = depth;
}