#pragma once

#include "../defs.hpp"

#include <vector>
#include <algorithm>

class Move;

class MoveList
{
private:
  typedef typename std::vector<Move> Moves;
  Moves _moves;

public:
  MoveList();

  typedef typename Moves::iterator iterator;
  typedef typename Moves::const_iterator const_iterator;

  void push_back(const Move &move);

  [[nodiscard]] Moves::size_type size() const;
  [[nodiscard]] const_iterator begin() const { return _moves.begin(); }
  [[nodiscard]] const_iterator cbegin() const { return _moves.cbegin(); }
  [[nodiscard]] const_iterator end() const { return _moves.end(); }
  [[nodiscard]] const_iterator cend() const { return _moves.cend(); }

  iterator begin() { return _moves.begin(); }
  iterator end() { return _moves.end(); }

  template <typename Compare>
  void sort(const Compare compare)
  {
    std::sort(_moves.begin(), _moves.end(), compare);
  }
};