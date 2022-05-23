#include "move.hpp"

std::string Move::getUCI() const
{
  std::string from_square = std::string(SQUARE_NAMES[this->getFromSquare()]);
  std::string to_square = std::string(SQUARE_NAMES[this->getToSquare()]);
  std::string promoted_piece = std::string(PIECE_REPR[this->getPromotedPiece() + 6]);
  std::string uci = from_square + to_square;
  uci = this->getPromotedPiece() ? uci + promoted_piece : uci;
  return uci;
}

Move::Move(int source_square, int target_square, int piece, int promoted_piece, bool capture, bool double_push, bool en_passant, bool castle)
{
  _move_encoded = source_square | (target_square << 6) | (piece << 12) | (promoted_piece << 16) | (capture << 20) | (double_push << 21) | (en_passant << 22) | (castle << 23);
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
  return ((_move_encoded & 0xf000) >> 12);
}

int Move::getPromotedPiece() const
{
  return ((_move_encoded & 0xf0000) >> 16);
}

bool Move::isCapture() const
{
  return (_move_encoded & 0x100000);
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

int Move::getEncoded() const
{
  return _move_encoded;
}