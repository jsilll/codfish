#pragma once

#include <vector>

#include <codchess/board.hpp>
#include <codchess/move.hpp>

namespace codchess::movegen {

/// @brief A list of moves.
class MoveList {
  public:
    /// @brief Returns an iterator to the beginning of the list.
    /// @return An iterator to the beginning of the list.
    [[nodiscard]] auto begin() const noexcept { return moves.begin(); }

    /// @brief Returns an iterator to the end of the list.
    /// @return An iterator to the end of the list.
    [[nodiscard]] auto end() const noexcept { return moves.begin() + size; }

    /// @brief Returns whehter the list is empty.
    /// @return True if the list is empty, false otherwise.
    [[nodiscard]] bool Empty() const noexcept { return size == 0; }

    /// @brief Inserts a move into the list.
    /// @param move The move to insert.
    void Insert(Move move) noexcept { moves[size++] = move; }

  private:
    /// @brief The maximum number of moves that can be generated on a board.
    static constexpr int kMaxMoves = 256;

    /// @brief The number of moves in the list.
    std::size_t size{0};
    /// @brief The moves array
    std::array<Move, kMaxMoves> moves;
};

/// @brief Returns all pseudo-legal moves on the board.
/// @param board The board to generate pseudo-legal moves on.
/// @return All pseudo-legal moves on the board.
[[nodiscard]] MoveList PseudoLegal(const Board &board) noexcept;

/// @brief Returns all pseudo-legal captures on the board.
/// @param board The board to generate pseudo-legal captures on.
/// @return All pseudo-legal captures on the board.
[[nodiscard]] MoveList PseudoLegalCaptures(const Board &board) noexcept;

/// @brief Returns all legal moves on the board.
/// @param board The board to generate legal moves on.
/// @return All legal moves on the board.
[[maybe_unused]] [[nodiscard]] MoveList Legal(Board &board) noexcept;

/// @brief Returns all legal captures on the board.
/// @param board The board to generate legal captures on.
/// @return All legal captures on the board.
[[maybe_unused]] [[nodiscard]] MoveList LegalCaptures(Board &board) noexcept;

/// @brief Returns whether a board has any legal moves to be played by the
/// @brief current player.
/// @param board The board to check for legal moves.
/// @return True if the board has legal moves, false otherwise.
[[maybe_unused]] [[nodiscard]] bool HasLegal(Board &board) noexcept;
}   // namespace codchess::movegen
