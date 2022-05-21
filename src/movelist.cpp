#include "move.hpp"

#include "movelist.hpp"

MoveList::MoveList()
{
  _moves.reserve(256);
}

void MoveList::push_back(const Move &move)
{
  _moves.push_back(move);
}

MoveList::MoveVector::size_type MoveList::size() const
{
  return _moves.size();
}