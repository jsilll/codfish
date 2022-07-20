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

  struct GameState
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
  bool isAscii() const;
  bool isWhiteOnBottom() const;

  GameState getState() const;
  std::string getFen() const;

  // Modifiers
  void setEnPassantSquare(int sq);
  void setCastlingRights(int castling_rights);
  void setFiftyMove(int fifty_move);
  void setState(GameState state);

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
  void unmakeMove(const Move &move, const GameState &info_board);

  // Display
  void display() const;
  bool toggleAscii();
  bool rotateDisplay();
};

inline bool operator==(const Board::Piece &p1, const Board::Piece &p2)
{
  return p1.color == p2.color && p1.type == p2.type;
}

inline bool operator!=(const Board::Piece &p1, const Board::Piece &p2)
{
  return p1.color != p2.color || p1.type != p2.type;
}

inline bool operator==(const Board::GameState &s1, const Board::GameState &s2)
{
  return s1.castling_rights == s2.castling_rights && s1.en_passant_square == s2.en_passant_square && s1.half_move_clock == s2.half_move_clock;
}

inline bool operator!=(const Board::GameState &s1, const Board::GameState &s2)
{
  return s1.castling_rights != s2.castling_rights || s1.en_passant_square != s2.en_passant_square || s1.half_move_clock != s2.half_move_clock;
}

inline bool operator==(const Board &b1, const Board &b2)
{
  if (b1.getSideToMove() != b2.getSideToMove())
  {
    return false;
  }
  if (b1.getCastlingRights() != b2.getCastlingRights())
  {
    return false;
  }
  if (b1.getEnPassantSquare() != b2.getEnPassantSquare())
  {
    return false;
  }
  if (b1.getHalfMoveClock() != b2.getHalfMoveClock())
  {
    return false;
  }
  if (b1.getFullMoveNumber() != b2.getFullMoveNumber())
  {
    return false;
  }
  if (b1.isAscii() != b2.isAscii())
  {
    return false;
  }
  if (b1.isWhiteOnBottom() != b2.isWhiteOnBottom())
  {
    return false;
  }

  for (int side = 0; side < N_SIDES; side++)
  {
    for (int piece = 0; piece < N_PIECES; piece++)
    {
      if (b1.getPieces(side, piece) != b2.getPieces(side, piece))
      {
        return false;
      }
    }
  }

  for (int side = 0; side < N_SIDES + 1; side++)
  {
    if (b1.getOccupancies(side) != b2.getOccupancies(side))
    {
      return false;
    }
  }

  for (int square = 0; square < N_SQUARES; square++)
  {
    if (b1.getPieceFromSquare(square) != b2.getPieceFromSquare(square))
    {
      return false;
    }
  }

  return true;
}
