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

  struct info
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

  info getBoardInfo() const;

  std::string getFen() const;

  // Modifiers
  void setEnPassantSquare(int sq);
  void setCastlingRights(int castling_rights);
  void setFiftyMove(int fifty_move);

  void clear();
  void setStartingPosition();
  void setFromFen(std::string const &piece_placements,
                  std::string const &active_color,
                  std::string const &castling_rights,
                  std::string const &en_passant,
                  std::string const &halfmove_clock,
                  std::string const &fullmove_number);
  int switchSideToMove();
  void makeMove(Move const &move);
  void unmakeMove(Move const &move, info const info_board);

  // Display
  void display() const;
  bool toggleAscii();
  bool rotateDisplay();
};
