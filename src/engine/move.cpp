#include <engine/move.hpp>

std::string Move::get_uci() const
{
  if (this->is_promotion())
  {
    return SQUARE_NAMES[this->get_from_square()] + SQUARE_NAMES[this->get_to_square()] + PIECE_REPR[this->get_promoted_piece() + 6];
  }
  return SQUARE_NAMES[this->get_from_square()] + SQUARE_NAMES[this->get_to_square()];
}

Move::Move(int source_square, int target_square, int piece, int captured_piece, int promoted_piece, bool is_double_push, bool is_en_passant, bool is_castle)
{
  _move_encoded = source_square | (target_square << 6) | (piece << 12) | (captured_piece << 15) | (promoted_piece << 18) | (is_double_push << 21) | (is_en_passant << 22) | (is_castle << 23);
}

int Move::get_from_square() const
{
  return (_move_encoded & 0x3f);
}

int Move::get_to_square() const
{
  return ((_move_encoded & 0xfc0) >> 6);
}

int Move::get_piece() const
{
  return ((_move_encoded & 0x7000) >> 12);
}

int Move::get_captured_piece() const
{
  return ((_move_encoded & 0x38000) >> 15);
}

int Move::get_promoted_piece() const
{
  return ((_move_encoded & 0x1C0000) >> 18);
}

bool Move::is_double_push() const
{
  return (_move_encoded & 0x200000);
}

bool Move::is_en_passant() const
{
  return (_move_encoded & 0x400000);
}

bool Move::is_castle() const
{
  return (_move_encoded & 0x800000);
}

bool Move::is_capture() const
{
  return this->get_captured_piece() != EMPTY_PIECE;
}

bool Move::is_promotion() const
{
  return this->get_promoted_piece() != EMPTY_PIECE;
}

int Move::get_encoded() const
{
  return _move_encoded;
}
