#pragma once

#include <string>
#include <iomanip>
#include <cstring>
#include <iostream>

#include <codlib/utils.hpp>
#include <codlib/bitboard.hpp>
#include <codlib/constants.hpp>

// Forward declarations
class Move;

/// @brief Represents a chess board
class Board
{
public:
  /// @brief A piece on the board
  struct Piece
  {
    /// @brief The type of the piece
    PieceType type;
    /// @brief The color of the piece
    Color color;

    friend constexpr bool operator==(const Piece &p1, const Piece &p2)
    {
      return p1.color == p2.color && p1.type == p2.type;
    }

    friend constexpr bool operator!=(const Piece &p1, const Piece &p2)
    {
      return p1.color != p2.color || p1.type != p2.type;
    }
  };

  /// @brief The auxiliary information of the board
  struct GameState
  {
    /// @brief The en passant square
    Square en_passant_square;
    /// @brief The castling rights
    int castling_rights;
    /// @brief The half move clock
    int half_move_clock;
    /// @brief The hash key
    u64 hash_key;

    friend constexpr bool operator==(const GameState &s1, const GameState &s2)
    {
      return s1.castling_rights == s2.castling_rights &&
             s1.en_passant_square == s2.en_passant_square &&
             s1.half_move_clock == s2.half_move_clock;
    }

    friend constexpr bool operator!=(const GameState &s1, const GameState &s2)
    {
      return s1.castling_rights != s2.castling_rights ||
             s1.en_passant_square != s2.en_passant_square ||
             s1.half_move_clock != s2.half_move_clock;
    }
  };

  /// @brief Default constructor
  /// @details Sets the board to the starting position
  [[nodiscard]] Board() noexcept
  {
    set_starting_position();
  }

  /// @brief Copy constructor
  /// @param board The board to copy
  [[nodiscard]] Board(const Board &board) noexcept = default;

  /// @brief Returns whether the board is in ASCII mode
  /// @return Whether the board is in ASCII mode
  [[nodiscard]] constexpr bool is_ascii() const noexcept
  {
    return _ascii;
  }

  /// @brief Returns the hash key of the board
  /// @return The hash key of the board
  [[nodiscard]] constexpr u64 get_hash_key() const noexcept
  {
    return _hash_key;
  }

  /// @brief Returns the current opponent color
  /// @return The current opponent color
  [[nodiscard]] constexpr Color get_opponent() const noexcept
  {
    return utils::get_opponent(_to_move);
  }

  /// @brief Returns the current game state
  /// @return The current game state
  [[nodiscard]] constexpr GameState get_state() const noexcept
  {
    return GameState{_en_passant_square, _castling_rights, _half_move_clock, _hash_key};
  }

  /// @brief Returns the current side to move
  /// @return The current side to move
  [[nodiscard]] constexpr Color get_side_to_move() const noexcept
  {
    return _to_move;
  }

  /// @brief Returns the castling rights
  /// @return The castling rights
  [[nodiscard]] constexpr int get_castling_rights() const noexcept
  {
    return _castling_rights;
  }

  /// @brief Returns the half move clock
  /// @return The half move clock
  [[nodiscard]] constexpr int get_half_move_clock() const noexcept
  {
    return _half_move_clock;
  }

  /// @brief Returns whether the white pieces are on the bottom
  /// @return Whether the white pieces are on the bottom
  [[nodiscard]] constexpr bool is_white_on_bottom() const noexcept
  {
    return _white_on_bottom;
  }

  /// @brief Returns the full move number
  /// @return The full move number
  [[nodiscard]] constexpr int get_full_move_number() const noexcept
  {
    return _full_move_number;
  }

  /// @brief Returns the en passant square
  /// @return The en passant square
  [[nodiscard]] constexpr Square get_en_passant_square() const noexcept
  {
    return _en_passant_square;
  }

  /// @brief Returns the occupancy of a color
  /// @param color The color
  /// @return The occupancy of the color
  [[nodiscard]] constexpr u64 get_occupancies(Color color) const noexcept
  {
    return _occupancies[color];
  }

  /// @brief Returns the piece on a square
  /// @param sq The square
  /// @return The piece on the square
  [[nodiscard]] constexpr Piece get_piece_from_square(Square sq) const noexcept
  {
    return _square[sq];
  }

  /// @brief Returns the pieces bitboard of a color and type
  /// @param color The color
  /// @param type The type
  [[nodiscard]] constexpr u64 get_pieces(Color color, PieceType type) const noexcept
  {
    return _pieces[color][type];
  }

  /// @brief Returns the FEN string of the board
  /// @return The FEN string of the board
  [[nodiscard]] std::string get_fen() const noexcept;

  /// @brief Returns whether a square is attacked by a color
  /// @param sq The square
  /// @param attacker The color
  /// @return Whether the square is attacked by the color
  [[nodiscard]] bool is_square_attacked(Square sq, Color attacker) const noexcept;

  /// @brief Clears the board
  void clear() noexcept
  {
    _to_move = WHITE;
    _castling_rights = 0;
    _half_move_clock = 0;
    _full_move_number = 0;
    _white_on_bottom = true;
    _hash_key = bitboard::kZERO;
    _en_passant_square = EMPTY_SQUARE;
    _occupancies[BOTH] = bitboard::kZERO;

    for (int color = WHITE; color < BOTH; ++color)
    {
      for (int piece_type = PAWN; piece_type < N_PIECES; ++piece_type)
      {
        _pieces[color][piece_type] = bitboard::kZERO;
      }
      _occupancies[color] = bitboard::kZERO;
    }

    for (int sq = A1; sq < N_SQUARES; ++sq)
    {
      _square[sq].type = EMPTY_PIECE;
      _square[sq].color = BLACK;
    }
  }

  /// @brief Sets the board to the starting position
  void set_starting_position() noexcept
  {
    const std::string initial_pos = "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR";
    set_from_fen(initial_pos, "w", "KQkq", "-", "0", "1");
  }

  /// @brief Toggles the ASCII mode
  /// @return Whether the board is in ASCII mode
  constexpr bool toggle_ascii() noexcept
  {
    return _ascii = !_ascii;
  }

  /// @brief Sets the current game state
  /// @param state The game state
  void set_state(const GameState &state) noexcept
  {
    _hash_key = state.hash_key;
    _castling_rights = state.castling_rights;
    _half_move_clock = state.half_move_clock;
    _en_passant_square = state.en_passant_square;
  }

  /// @brief Rotates the board
  /// @return Whether white is on the bottom
  constexpr bool rotate_display() noexcept
  {
    return _white_on_bottom = !_white_on_bottom;
  }

  /// @brief Switches the side to move
  /// @return The new side to move
  constexpr int switch_side_to_move() noexcept
  {
    return _to_move = get_opponent();
  }

  /// @brief Sets the current en passant square
  /// @param sq The en passant square
  void set_en_passant_square(Square sq) noexcept
  {
    _en_passant_square = sq;
  }

  /// @brief Sets the current castling rights
  /// @param castling_rights The castling rights
  void set_castling_rights(int castling_rights) noexcept
  {
    _castling_rights = castling_rights;
  }

  /// @brief Makes a move on the board
  /// @param move The move
  /// @note This function does not check if the move is legal
  void make(const Move move) noexcept;

  /// @brief Unmakes a move on the board
  /// @param move The move
  /// @param info_board The previous game state
  void unmake(const Move move, const GameState info_board) noexcept;

  /// @brief Sets the board from a FEN string
  /// @param piece_placements The piece placements
  /// @param active_color The active color
  /// @param castling_rights The castling rights
  /// @param en_passant The en passant square
  /// @param halfmove_clock The half move clock
  /// @param fullmove_number The full move number
  void set_from_fen(const std::string &piece_placements,
                    const std::string &active_color,
                    const std::string &castling_rights,
                    const std::string &en_passant,
                    const std::string &halfmove_clock,
                    const std::string &fullmove_number) noexcept;

  /// @brief Displays the board on the console
  void display() const noexcept
  {
    // TODO: Implement cout operator
    int offset = _ascii ? 0 : 13;
    std::cout << '\n';
    if (!_white_on_bottom)
    {
      std::cout << "      h   g   f   e   d   c   b   a\n";
      for (int rank = RANK_1; rank < RANK_8; rank++)
      {
        std::cout << "    +---+---+---+---+---+---+---+---+\n"
                  << "    |";
        for (int file = FILE_H; file >= FILE_A; file--)
        {
          struct Piece piece = _square[utils::get_square((Rank)rank, (File)file)];
          std::cout << " " << PIECE_REPR[piece.type + offset + (6 * piece.color)] << " |";
        }
        std::cout << std::setw(3) << rank + 1 << "\n";
      }
      std::cout << "    +---+---+---+---+---+---+---+---+\n";
    }
    else
    {
      for (int rank = RANK_8; rank >= RANK_1; rank--)
      {
        std::cout << "    +---+---+---+---+---+---+---+---+\n"
                  << std::setw(3) << rank + 1 << " |";

        for (int file = 0; file < 8; file++)
        {
          struct Piece piece = _square[utils::get_square((Rank)rank, (File)file)];
          std::cout << " " << PIECE_REPR[piece.type + offset + (6 * piece.color)] << " |";
        }
        std::cout << '\n';
      }
      std::cout << "    +---+---+---+---+---+---+---+---+\n"
                << "      a   b   c   d   e   f   g   h\n";
    }
    std::cout << std::endl;
  }

  friend constexpr bool operator==(const Board &b1, const Board &b2)
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
        if (b1.get_pieces(static_cast<Color>(side), static_cast<PieceType>(piece)) != b2.get_pieces(static_cast<Color>(side), static_cast<PieceType>(piece)))
        {
          return false;
        }
      }
    }

    for (int side = 0; side < N_SIDES + 1; side++)
    {
      if (b1.get_occupancies(static_cast<Color>(side)) != b2.get_occupancies(static_cast<Color>(side)))
      {
        return false;
      }
    }

    for (int square = 0; square < N_SQUARES; square++)
    {
      if (b1.get_piece_from_square(static_cast<Square>(square)) != b2.get_piece_from_square(static_cast<Square>(square)))
      {
        return false;
      }
    }

    return true;
  }

private:
  /// @brief Whether the board is in ASCII mode
  bool _ascii{};
  /// @brief The hash key of the board
  u64 _hash_key{};
  /// @brief The side to move
  Color _to_move{};
  /// @brief The current castling rights
  int _castling_rights{};
  /// @brief The half move clock
  int _half_move_clock{};
  /// @brief The full move number
  int _full_move_number{};
  /// @brief Whether the white pieces are on the bottom
  bool _white_on_bottom{};
  /// @brief The squares representation of the board
  /// @details This is always maintains the same state as the bitboards
  Piece _square[N_SQUARES]{};
  /// @brief The en passant square
  Square _en_passant_square{};
  /// @brief The occupancy bitboards
  u64 _occupancies[N_SIDES + 1]{};
  /// @brief The bitboards for the pieces
  u64 _pieces[N_SIDES][N_PIECES]{};

  /// @brief Updates the occupancy bitboards
  /// @details This function is called after a move is made
  void update_occupancies() noexcept;

  /// @brief Updates the bitboards from the squares
  /// @details This function is called when setting the board from a FEN string
  void update_bitboards_from_squares() noexcept;
};