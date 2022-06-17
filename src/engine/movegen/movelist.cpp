#include "movelist.hpp"

#include "move.hpp"

#include <algorithm>

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

void MoveList::sort()
{
  std::sort(_moves.begin(), _moves.end(), std::greater<Move>());
}
