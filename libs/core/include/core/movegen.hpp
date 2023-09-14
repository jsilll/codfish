#pragma once

#include <vector>

#include <core/board.hpp>
#include <core/move.hpp>

namespace codfish::core::movegen {
/// @brief A list of moves.
class MoveList final {
  public:
    /// @brief Returns an iterator to the beginning of the list.
    /// @return An iterator to the beginning of the list.
    [[nodiscard]] constexpr auto begin() const noexcept {
        return _moves.begin();
    }

    /// @brief Returns an iterator to the beginning of the list.
    /// @return An iterator to the beginning of the list.
    [[nodiscard]] auto begin() noexcept { return _moves.begin(); }

    /// @brief Returns an iterator to the end of the list.
    /// @return An iterator to the end of the list.
    [[nodiscard]] constexpr auto end() const noexcept {
        return _moves.begin() + _size;
    }

    /// @brief Returns an iterator to the end of the list.
    /// @return An iterator to the end of the list.
    [[nodiscard]] auto end() noexcept { return _moves.begin() + _size; }

    /// @brief Returns the number of moves in the list.
    /// @return The number of moves in the list.
    [[nodiscard]] auto size() const noexcept { return _size; }

    /// @brief Returns whehter the list is empty.
    /// @return True if the list is empty, false otherwise.
    [[nodiscard]] constexpr bool empty() const noexcept { return _size == 0; }

    /// @brief Inserts a move into the list.
    /// @param move The move to insert.
    void Insert(const Move move) noexcept { _moves[_size++] = move; }

  private:
    /// @brief The number of moves in the list.
    std::size_t _size{0};
    /// @brief The moves array
    std::array<Move, 256> _moves{};
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
}   // namespace codfish::core::movegen