#include "move.hpp"

int mvv_lva[6][6] = {
    105,
    205,
    305,
    405,
    505,
    605,
    104,
    204,
    304,
    404,
    504,
    604,
    103,
    203,
    303,
    403,
    503,
    603,
    102,
    202,
    302,
    402,
    502,
    602,
    101,
    201,
    301,
    401,
    501,
    601,
    100,
    200,
    300,
    400,
    500,
    600,
};

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
  _move_encoded = source_square | (target_square << 6) | (piece << 12) | (promoted_piece << 15) | (capture << 18) | (double_push << 19) | (en_passant << 20) | (castle << 21);
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
  return ((_move_encoded & 0x7000) >> 15);
}

bool Move::isCapture() const
{
  return (_move_encoded & 0x40000);
}

bool Move::isDoublePush() const
{
  return (_move_encoded & 0x80000);
}

bool Move::isEnPassant() const
{
  return (_move_encoded & 0x100000);
}

bool Move::isCastle() const
{
  return (_move_encoded & 0x200000);
}

int Move::getEncoded() const
{
  return _move_encoded;
}

int Move::scoreMove() const
{
  if (this->isCapture())
  {
    return mvv_lva[6][6];
  }
  return 0;
}