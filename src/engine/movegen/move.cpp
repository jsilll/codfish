#include <engine/movegen/move.hpp>

std::string Move::getUCI() const
{
  if (this->isPromotion())
  {
   return SQUARE_NAMES[this->getFromSquare()] + SQUARE_NAMES[this->getToSquare()] + PIECE_REPR[this->getPromotedPiece() + 6];
  }
  return SQUARE_NAMES[this->getFromSquare()] + SQUARE_NAMES[this->getToSquare()];
}

Move::Move(int source_square, int target_square, int piece, int captured_piece, int promoted_piece, bool is_double_push, bool is_en_passant, bool is_castle)
{
  _move_encoded = source_square | (target_square << 6) | (piece << 12) | (captured_piece << 15) | (promoted_piece << 18) | (is_double_push << 21) | (is_en_passant << 22) | (is_castle << 23);
}

int Move::getFromSquare() const
{
  return (_move_encoded & 0x3f);
}

int Move::getToSquare() const
{
  return ((_move_encoded & 0xfc0) >> 6);
}

int Move::getPiece() const
{
  return ((_move_encoded & 0x7000) >> 12);
}

int Move::getCapturedPiece() const
{
  return ((_move_encoded & 0x38000) >> 15);
}

int Move::getPromotedPiece() const
{
  return ((_move_encoded & 0x1C0000) >> 18);
}

bool Move::isDoublePush() const
{
  return (_move_encoded & 0x200000);
}

bool Move::isEnPassant() const
{
  return (_move_encoded & 0x400000);
}

bool Move::isCastle() const
{
  return (_move_encoded & 0x800000);
}

bool Move::isCapture() const
{
  return this->getCapturedPiece() != EMPTY;
}

bool Move::isPromotion() const
{
  return this->getPromotedPiece() != EMPTY;
}

int Move::getEncoded() const
{
  return _move_encoded;
}
