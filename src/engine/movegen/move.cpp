#include "move.hpp"

// clang-format off
int mvv_lva[6][6] = {
    {105, 205, 305, 405, 505, 605}, 
    {104, 204, 304, 404, 504, 604},
    {103, 203, 303, 403, 503, 603},
    {102, 202, 302, 402, 502, 602},
    {101, 201, 301, 401, 501, 601},
    {100, 200, 300, 400, 500, 600}
};
// clang-format on

std::string Move::getUCI() const
{
  std::string from_square = std::string(SQUARE_NAMES[this->getFromSquare()]);
  std::string to_square = std::string(SQUARE_NAMES[this->getToSquare()]);
  std::string promoted_piece = std::string(PIECE_REPR[this->getPromotedPiece() + 6]);
  std::string uci = from_square + to_square;
  uci = this->getPromotedPiece() ? uci + promoted_piece : uci;
  return uci;
}

Move::Move(int source_square, int target_square, int piece, int captured_piece, int promoted_piece, bool capture, bool double_push, bool en_passant, bool castle)
{
  _move_encoded = source_square | (target_square << 6) | (piece << 12) | (captured_piece << 15) | (promoted_piece << 18) | (capture << 21) | (double_push << 22) | (en_passant << 23) | (castle << 24);
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

bool Move::isCapture() const
{
  return (_move_encoded & 0x200000);
}

bool Move::isDoublePush() const
{
  return (_move_encoded & 0x400000);
}

bool Move::isEnPassant() const
{
  return (_move_encoded & 0x800000);
}

bool Move::isCastle() const
{
  return (_move_encoded & 0x1000000);
}

int Move::getEncoded() const
{
  return _move_encoded;
}

int Move::score() const
{
  if (this->isCapture())
  {
    return mvv_lva[this->getPiece()][this->getCapturedPiece()];
  }
  return 0;
}

bool Move::operator>(const Move &move) const
{
  return (this->score() > move.score());
}