#pragma once

#include <engine/defs.hpp>

class Move;

class Board
{
public:
  struct Piece
  {
    int type;
    int color;
  };

  struct State
  {
    int en_passant_square;
    int castling_rights;
    int half_move_clock;
  };

private:
  int _to_move;
  int _castling_rights;
  int _en_passant_square;
  int _half_move_clock;
  int _full_move_number;

  U64 _pieces[N_SIDES][N_PIECES];
  U64 _occupancies[N_SIDES + 1];

  bool _ascii;
  bool _white_on_bottom;
  Piece _square[N_SQUARES];

  void updateOccupancies();
  void updateBBFromSquares();

public:
  Board();
  Board(const Board &board);

  // Getters
  U64 getPieces(int color, int type) const;
  U64 getOccupancies(int color) const;

  int getSideToMove() const;
  int getOpponent() const;
  int getCastlingRights() const;
  int getEnPassantSquare() const;
  int getHalfMoveClock() const;
  int getFullMoveNumber() const;
  Piece getPieceFromSquare(int sq) const;
  bool isSquareAttacked(int sq, int attacker_side) const;

  State getState() const;

  std::string getFen() const;

  // Modifiers
  void setEnPassantSquare(int sq);
  void setCastlingRights(int castling_rights);
  void setFiftyMove(int fifty_move);
  void setState(State state);

  void clear();
  void setStartingPosition();
  void setFromFen(const std::string &piece_placements,
                  const std::string &active_color,
                  const std::string &castling_rights,
                  const std::string &en_passant,
                  const std::string &halfmove_clock,
                  const std::string &fullmove_number);
  int switchSideToMove();
  void makeMove(const Move &move);
  void unmakeMove(const Move &move, State const info_board);

  // Display
  void display() const;
  bool toggleAscii();
  bool rotateDisplay();
};
