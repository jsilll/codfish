#pragma once

#include <engine/constants.hpp>

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
    u64 hash_key;
  };

private:
  int _to_move;
  int _castling_rights;
  int _en_passant_square;
  int _half_move_clock;
  int _full_move_number;
  u64 _hash_key;

  u64 _pieces[N_SIDES][N_PIECES];
  u64 _occupancies[N_SIDES + 1];

  bool _ascii;
  bool _white_on_bottom;
  Piece _square[N_SQUARES];

  void update_occupancies();
  void update_bb_from_squares();

public:
  Board();
  Board(const Board &board);

  u64 get_pieces(int color, int type) const;
  u64 get_occupancies(int color) const;
  int get_side_to_move() const;
  int get_opponent() const;
  int get_castling_rights() const;
  int get_en_passant_square() const;
  int get_half_move_clock() const;
  int get_full_move_number() const;
  Piece get_piece_from_square(int sq) const;
  bool is_square_attacked(int sq, int attacker_side) const;
  bool is_ascii() const;
  bool is_white_on_bottom() const;
  GameState get_state() const;
  std::string get_fen() const;

  void set_en_passant_square(int sq);
  void set_castling_rights(int castling_rights);
  void set_fifty_move(int fifty_move);
  void set_state(GameState state);

  u64 get_hash_key() const;

  void clear();
  void set_starting_position();
  void set_from_fen(const std::string &piece_placements,
                    const std::string &active_color,
                    const std::string &castling_rights,
                    const std::string &en_passant,
                    const std::string &halfmove_clock,
                    const std::string &fullmove_number);
  int switch_side_to_move();
  void make_move(const Move move);
  void unmake_move(const Move move, const GameState info_board);

  void display() const;
  bool toggle_ascii();
  bool rotate_display();
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
  if (b1.get_side_to_move() != b2.get_side_to_move())
  {
    return false;
  }
  if (b1.get_castling_rights() != b2.get_castling_rights())
  {
    return false;
  }
  if (b1.get_en_passant_square() != b2.get_en_passant_square())
  {
    return false;
  }
  if (b1.get_half_move_clock() != b2.get_half_move_clock())
  {
    return false;
  }
  if (b1.get_full_move_number() != b2.get_full_move_number())
  {
    return false;
  }
  if (b1.is_ascii() != b2.is_ascii())
  {
    return false;
  }
  if (b1.is_white_on_bottom() != b2.is_white_on_bottom())
  {
    return false;
  }

  for (int side = 0; side < N_SIDES; side++)
  {
    for (int piece = 0; piece < N_PIECES; piece++)
    {
      if (b1.get_pieces(side, piece) != b2.get_pieces(side, piece))
      {
        return false;
      }
    }
  }

  for (int side = 0; side < N_SIDES + 1; side++)
  {
    if (b1.get_occupancies(side) != b2.get_occupancies(side))
    {
      return false;
    }
  }

  for (int square = 0; square < N_SQUARES; square++)
  {
    if (b1.get_piece_from_square(square) != b2.get_piece_from_square(square))
    {
      return false;
    }
  }

  return true;
}
