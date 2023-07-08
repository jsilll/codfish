#pragma once

#include <string>

#include <codchess/base.hpp>
#include <codchess/utils.hpp>

namespace cod::chess {
/// @brief Encodes a chess move
class Move final {
  public:
    /// @brief Default constructor
    [[maybe_unused]] [[nodiscard]] Move() noexcept = default;

    /// @brief Constructor
    /// @param source_square The source square
    /// @param target_square The target square
    /// @param piece The piece
    /// @param captured_piece The captured piece
    /// @param promoted_piece The promoted piece
    /// @param is_double_push If if double push
    /// @param is_en_passant If is en passant
    /// @param is_castle If is castle
    [[nodiscard]] Move(const Square source_square, const Square target_square,
                       const Piece piece, const Piece captured_piece,
                       const Piece promoted_piece, const bool is_double_push,
                       const bool is_en_passant, const bool is_castle) noexcept
        : _encoded(static_cast<std::uint32_t>(source_square) |
                   static_cast<std::uint32_t>(target_square) << 6 |
                   static_cast<std::uint32_t>(piece) << 12 |
                   static_cast<std::uint32_t>(captured_piece) << 15 |
                   static_cast<std::uint32_t>(promoted_piece) << 18 |
                   static_cast<std::uint32_t>(is_double_push) << 21 |
                   static_cast<std::uint32_t>(is_en_passant) << 22 |
                   static_cast<std::uint32_t>(is_castle) << 23) {}

    /// @brief Returns the source square
    /// @return The source square
    [[maybe_unused]] [[nodiscard]] constexpr auto FromSquare() const noexcept {
        return static_cast<Square>(_encoded & 0x3f);
    }

    /// @brief Returns the target square
    /// @return The target square
    [[maybe_unused]] [[nodiscard]] constexpr auto ToSquare() const noexcept {
        return static_cast<Square>((_encoded & 0xfc0) >> 6);
    }

    /// @brief Returns the piece
    /// @return The piece
    [[maybe_unused]] [[nodiscard]] constexpr auto MovedPiece() const noexcept {
        return static_cast<Piece>((_encoded & 0x7000) >> 12);
    }

    /// @brief Returns the captured piece
    /// @return The captured piece
    [[maybe_unused]] [[nodiscard]] constexpr auto
    CapturedPiece() const noexcept {
        return static_cast<Piece>((_encoded & 0x38000) >> 15);
    }

    /// @brief Returns the promoted piece
    /// @return The promoted piece
    [[maybe_unused]] [[nodiscard]] constexpr auto
    PromotedPiece() const noexcept {
        return static_cast<Piece>((_encoded & 0x1C0000) >> 18);
    }

    /// @brief Returns if it is a double push
    /// @return If it is a double push
    [[maybe_unused]] [[nodiscard]] constexpr bool
    IsDoublePush() const noexcept {
        return (_encoded & 0x200000);
    }

    /// @brief Returns if it is an en passant
    /// @return If it is an en passant
    [[maybe_unused]] [[nodiscard]] constexpr bool IsEnPassant() const noexcept {
        return (_encoded & 0x400000);
    }

    /// @brief Returns if it is a castle
    /// @return If it is a castle
    [[maybe_unused]] [[nodiscard]] constexpr bool IsCastle() const noexcept {
        return (_encoded & 0x800000);
    }

    /// @brief Returns if it is a capture
    /// @return If it is a capture
    [[maybe_unused]] [[nodiscard]] constexpr auto IsCapture() const noexcept {
        return CapturedPiece() != Piece::EMPTY_PIECE;
    }

    /// @brief Returns if it is a promotion
    /// @return If it is a promotion
    [[maybe_unused]] [[nodiscard]] constexpr auto IsPromotion() const noexcept {
        return PromotedPiece() != Piece::EMPTY_PIECE;
    }

    /// @brief Returns the move in UCI format
    /// @return The move in UCI format
    [[maybe_unused]] [[nodiscard]] std::string ToString() const noexcept {
        if (IsPromotion()) {
            return utils::SquareToString(FromSquare()) +
                   utils::SquareToString(ToSquare()) +
                   utils::PieceToString(PromotedPiece(), Color::BLACK);
        } else {
            return utils::SquareToString(FromSquare()) +
                   utils::SquareToString(ToSquare());
        }
    }

    /// @brief Returns the encoded move
    /// @return The encoded move
    [[maybe_unused]] [[nodiscard]] constexpr auto encoded() const noexcept {
        return _encoded;
    }

    /// @brief Overload of the == operator for the Move class
    /// @param m1 The move
    /// @param m2 The other move
    /// @return Whether the moves are equal
    [[maybe_unused]] friend constexpr auto operator==(const Move &m1,
                                                      const Move &m2) noexcept {
        return m1._encoded == m2._encoded;
    }

    /// @brief Overload of the != operator for the Move class
    /// @param m1 The move
    /// @param m2 The other move
    /// @return Whether the moves are not equal
    [[maybe_unused]] friend constexpr auto operator!=(const Move &m1,
                                                      const Move &m2) noexcept {
        return m1._encoded != m2._encoded;
    }

  private:
    /// @brief The encoded move for memory efficiency
    std::uint32_t _encoded{0};
};
}   // namespace cod::chess