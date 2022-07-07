#pragma once

#include <engine/defs.hpp>

#include <iostream>
#include <string>
#include <vector>

class Move
{
private:
  int _move_encoded;

public:
  Move() : _move_encoded(0){};
  explicit Move(int move_encoded) : _move_encoded(move_encoded){};
  Move(int source_square, int target_square, int piece, int captured_piece, int promoted_piece, bool is_double_push, bool is_en_passant, bool is_castle);

  [[nodiscard]] int getFromSquare() const;
  [[nodiscard]] int getToSquare() const;
  [[nodiscard]] int getPiece() const;
  [[nodiscard]] int getCapturedPiece() const;
  [[nodiscard]] int getPromotedPiece() const;
  [[nodiscard]] bool isDoublePush() const;
  [[nodiscard]] bool isEnPassant() const;
  [[nodiscard]] bool isCastle() const;
  [[nodiscard]] bool isCapture() const;
  [[nodiscard]] bool isPromotion() const;
  [[nodiscard]] int getEncoded() const;

  [[nodiscard]] std::string getUCI() const;
};