#include "../defs.hpp"

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
  Move(int source_square, int target_square, int piece, int captured_piece, int promoted_piece, bool is_double_push, bool is_en_passant, bool is_castle);

  int getFromSquare() const;
  int getToSquare() const;
  int getPiece() const;
  int getCapturedPiece() const;
  int getPromotedPiece() const;
  bool isDoublePush() const;
  bool isEnPassant() const;
  bool isCastle() const;
  bool isCapture() const;
  bool isPromotion() const;
  int getEncoded() const;

  std::string getUCI() const;
};