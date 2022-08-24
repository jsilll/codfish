#pragma once

#include <engine/constants.hpp>

#include <iostream>
#include <string>
#include <vector>

class Move
{
private:
  uint32_t _move_encoded;

public:
  Move() : _move_encoded(0){};
  Move(int source_square, int target_square, int piece, int captured_piece, int promoted_piece, bool is_double_push, bool is_en_passant, bool is_castle);

  [[nodiscard]] Square get_from_square() const;
  [[nodiscard]] Square get_to_square() const;
  [[nodiscard]] PieceType get_piece_type() const;
  [[nodiscard]] PieceType get_captured_piece_type() const;
  [[nodiscard]] PieceType get_promoted_piece_type() const;
  [[nodiscard]] bool is_double_push() const;
  [[nodiscard]] bool is_en_passant() const;
  [[nodiscard]] bool is_castle() const;
  [[nodiscard]] bool is_capture() const;
  [[nodiscard]] bool is_promotion() const;
  [[nodiscard]] uint32_t get_encoded() const;

  [[nodiscard]] std::string get_uci() const;
};

inline bool operator==(const Move &m1, const Move &m2)
{
  return m1.get_encoded() == m2.get_encoded();
}

inline bool operator!=(const Move &m1, const Move &m2)
{
  return m1.get_encoded() != m2.get_encoded();
}