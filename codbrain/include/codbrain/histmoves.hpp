#pragma once

#include <codchess/base.hpp>

namespace codbrain {
/// @brief Class that implements the history moves
class HistMoves {
  public:
    /// @brief Returns the history value for the given color, piece and square
    /// @param color The color
    /// @param piece The piece
    /// @param square The square
    /// @return The history value
    [[nodiscard]] constexpr auto Get(const codchess::Color color,
                                     const codchess::Move move) const noexcept {
        return _history_moves[color][move.MovedPiece()][move.ToSquare()];
    }

    /// @brief Adds a move to the move history
    /// @param move The move
    constexpr void Add(const codchess::Color color, const codchess::Move move,
                       const std::uint32_t depth) noexcept {
        _history_moves[color][move.MovedPiece()][move.ToSquare()] += depth;
    }

    /// @brief Clears the history moves
    constexpr void Clear() noexcept {
        std::memset(_history_moves, 0, sizeof(_history_moves));
    }

  private:
    /// @brief History moves
    std::uint32_t _history_moves[codchess::N_COLORS][codchess::N_PIECES]
                                [codchess::N_SQUARES]{};
};
}   // namespace codbrain