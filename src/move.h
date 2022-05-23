#include "defs.h"

#include <iostream>
#include <string>
#include <vector>

class Move
{
private:
  int _move_encoded;

public:
  Move() : _move_encoded(0){};
  Move(int move_encoded) : _move_encoded(move_encoded){};
  Move(int source_square, int target_square, int piece, int promoted_piece, bool capture, bool double_push, bool en_passant, bool castle);

  int getFromSquare() const;
  int getToSquare() const;
  int getPiece() const;
  int getPromotedPiece() const;
  bool isCapture() const;
  bool isDoublePush() const;
  bool isEnPassant() const;
  bool isCastle() const;
  int getEncoded() const;

  std::string getUCI() const;
};