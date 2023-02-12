#pragma once

#include <iostream>
#include <string>
#include <vector>

#include <codlib/constants.hpp>

/// @brief Encodes a chess move
class Move
{
public:
  /// @brief Default constructor
  [[nodiscard]] Move() noexcept : _move_encoded(0){};

  /// @brief Constructor
  /// @param source_square The source square
  /// @param target_square The target square
  /// @param piece The piece
  /// @param captured_piece The captured piece
  /// @param promoted_piece The promoted piece
  /// @param is_double_push If if double push
  /// @param is_en_passant If is en passant
  /// @param is_castle If is castle
  [[nodiscard]] Move(int source_square,
                     int target_square,
                     int piece,
                     int captured_piece,
                     int promoted_piece,
                     bool is_double_push,
                     bool is_en_passant,
                     bool is_castle) noexcept
      : _move_encoded((std::uint32_t)source_square |
                      ((std::uint32_t)target_square << 6) |
                      ((std::uint32_t)piece << 12) |
                      ((std::uint32_t)captured_piece << 15) |
                      ((std::uint32_t)promoted_piece << 18) |
                      ((std::uint32_t)is_double_push << 21) |
                      ((std::uint32_t)is_en_passant << 22) |
                      ((std::uint32_t)is_castle << 23)) {}

  /// @brief Returns the source square
  /// @return The source square
  [[nodiscard]] constexpr Square get_from_square() const noexcept
  {
    return static_cast<Square>(_move_encoded & 0x3f);
  }

  /// @brief Returns the target square
  /// @return The target square
  [[nodiscard]] constexpr Square get_to_square() const noexcept
  {
    return static_cast<Square>((_move_encoded & 0xfc0) >> 6);
  }

  /// @brief Returns the piece
  /// @return The piece
  [[nodiscard]] constexpr PieceType get_piece_type() const noexcept
  {
    return static_cast<PieceType>((_move_encoded & 0x7000) >> 12);
  }

  /// @brief Returns the captured piece
  /// @return The captured piece
  [[nodiscard]] constexpr PieceType get_captured_piece_type() const noexcept
  {
    return static_cast<PieceType>((_move_encoded & 0x38000) >> 15);
  }

  /// @brief Returns the promoted piece
  /// @return The promoted piece
  [[nodiscard]] constexpr PieceType get_promoted_piece_type() const noexcept
  {
    return static_cast<PieceType>((_move_encoded & 0x1C0000) >> 18);
  }

  /// @brief Returns if it is a double push
  /// @return If it is a double push
  [[nodiscard]] constexpr bool is_double_push() const noexcept
  {
    return (_move_encoded & 0x200000);
  }

  /// @brief Returns if it is an en passant
  /// @return If it is an en passant
  [[nodiscard]] constexpr bool is_en_passant() const noexcept
  {
    return (_move_encoded & 0x400000);
  }

  /// @brief Returns if it is a castle
  /// @return If it is a castle
  [[nodiscard]] constexpr bool is_castle() const noexcept
  {
    return (_move_encoded & 0x800000);
  }

  /// @brief Returns if it is a capture
  /// @return If it is a capture
  [[nodiscard]] constexpr bool is_capture() const noexcept
  {
    return get_captured_piece_type() != EMPTY_PIECE;
  }

  /// @brief Returns if it is a promotion
  /// @return If it is a promotion
  [[nodiscard]] constexpr bool is_promotion() const noexcept
  {
    return get_promoted_piece_type() != EMPTY_PIECE;
  }

  /// @brief Returns the encoded move
  /// @return The encoded move
  [[nodiscard]] constexpr auto get_encoded() const noexcept
  {
    return _move_encoded;
  }

  /// @brief Returns the move in UCI format
  /// @return The move in UCI format
  [[nodiscard]] constexpr std::string get_uci() const noexcept
  {
    if (is_promotion())
    {
      return SQUARE_NAMES[get_from_square()] + SQUARE_NAMES[get_to_square()] + PIECE_REPR[get_promoted_piece_type() + 6];
    }
    return SQUARE_NAMES[get_from_square()] + SQUARE_NAMES[get_to_square()];
  }

  /// @brief Overload of the == operator for the Move class
  /// @param m1 The move
  /// @param m2 The other move
  /// @return Whether the moves are equal
  friend constexpr bool operator==(const Move &m1, const Move &m2)
  {
    return m1._move_encoded == m2._move_encoded;
  }

  /// @brief Overload of the != operator for the Move class
  /// @param m1 The move
  /// @param m2 The other move
  /// @return Whether the moves are not equal
  friend constexpr bool operator!=(const Move &m1, const Move &m2)
  {
    return m1._move_encoded != m2._move_encoded;
  }

private:
  /// @brief The encoded move for memory efficiency
  std::uint32_t _move_encoded;
};
