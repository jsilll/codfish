#pragma once

#include "../defs.hpp"

#include <vector>

class Move;

class MoveList
{
private:
  typedef typename std::vector<Move> MoveVector;
  MoveVector _moves;

public:
  MoveList();

  typedef typename MoveVector::iterator iterator;
  typedef typename MoveVector::const_iterator const_iterator;

  void push_back(const Move &move);

  [[nodiscard]] MoveVector::size_type size() const;
  [[nodiscard]] const_iterator begin() const { return _moves.begin(); }
  [[nodiscard]] const_iterator cbegin() const { return _moves.cbegin(); }
  [[nodiscard]] const_iterator end() const { return _moves.end(); }
  [[nodiscard]] const_iterator cend() const { return _moves.cend(); }

  iterator begin() { return _moves.begin(); }
  iterator end() { return _moves.end(); }

  void sort();
};