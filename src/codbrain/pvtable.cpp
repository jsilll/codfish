#include <codchess/movepicker/pvtable.hpp>

#include <codchess/move.hpp>

#include <cstring>
#include <algorithm>

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

std::vector<Move> PVTable::get_pv_from_depth(int start_depth)
{
    std::vector<Move> res;
    for (int i = start_depth; i < _length[start_depth]; i++)
    {
        res.push_back(_table[start_depth][i]);
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

void PVTable::add_pv_from_depth(std::vector<Move> moves, int starting_depth)
{
    int last_depth = starting_depth + (int)moves.size() - 1;

    // Update PV with moves from starting_depth to last depth (reverse order)
    for (int i = (int)moves.size() - 1; i >= 0; i--)
    {
        this->add(moves[(std::vector<Move>::size_type)i], last_depth--);
    }

    // Update the Higher Depths
    for (int current_depth = starting_depth - 1; current_depth >= 0; current_depth--)
    {
        memcpy(&_table[current_depth][current_depth + 1], &_table[current_depth + 1][current_depth + 1], (unsigned long)_length[current_depth + 1] * sizeof(int));
    }
}

void PVTable::clear()
{
    memset(_table, 0, sizeof(_table));
}

void PVTable::set_length(int depth)
{
    _length[depth] = depth;
}