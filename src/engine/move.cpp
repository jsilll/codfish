#include <engine/move.hpp>

std::string Move::get_uci() const
{
  if (this->is_promotion())
  {
    return SQUARE_NAMES[this->get_from_square()] + SQUARE_NAMES[this->get_to_square()] + PIECE_REPR[this->get_promoted_piece_type() + 6];
  }
  return SQUARE_NAMES[this->get_from_square()] + SQUARE_NAMES[this->get_to_square()];
}

Move::Move(int source_square, int target_square, int piece, int captured_piece, int promoted_piece, bool is_double_push, bool is_en_passant, bool is_castle)
{
  _move_encoded = (uint32_t)source_square | ((uint32_t)target_square << 6) | ((uint32_t)piece << 12) | ((uint32_t)captured_piece << 15) | ((uint32_t)promoted_piece << 18) | ((uint32_t)is_double_push << 21) | ((uint32_t)is_en_passant << 22) | ((uint32_t)is_castle << 23);
}

Square Move::get_from_square() const
{
  return (Square)(_move_encoded & 0x3f);
}

Square Move::get_to_square() const
{
  return (Square)((_move_encoded & 0xfc0) >> 6);
}

PieceType Move::get_piece_type() const
{
  return (PieceType)((_move_encoded & 0x7000) >> 12);
}

PieceType Move::get_captured_piece_type() const
{
  return (PieceType)((_move_encoded & 0x38000) >> 15);
}

PieceType Move::get_promoted_piece_type() const
{
  return (PieceType)((_move_encoded & 0x1C0000) >> 18);
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
  return this->get_captured_piece_type() != EMPTY_PIECE;
}

bool Move::is_promotion() const
{
  return this->get_promoted_piece_type() != EMPTY_PIECE;
}

uint32_t Move::get_encoded() const
{
  return _move_encoded;
}
