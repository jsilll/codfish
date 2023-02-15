#pragma once

#include <vector>

#include <codlib/board.hpp>
#include <codlib/move.hpp>

namespace movegen {
/// @brief Returns all pseudo-legal moves on the board.
/// @param board The board to generate pseudo-legal moves on.
/// @return All pseudo-legal moves on the board.
[[nodiscard]] std::vector<Move> generate_pseudo_legal_moves(const Board &board) noexcept;

/// @brief Returns all pseudo-legal captures on the board.
/// @param board The board to generate pseudo-legal captures on.
/// @return All pseudo-legal captures on the board.
[[nodiscard]] std::vector<Move> generate_pseudo_legal_captures(const Board &board) noexcept;

/// @brief Returns whether a board has any legal moves to be played by the current player.
/// @param board The board to check for legal moves.
/// @return True if the board has legal moves, false otherwise.
[[maybe_unused]] [[nodiscard]] bool has_legal_moves(const Board &board) noexcept;

/// @brief Returns all legal moves on the board.
/// @param board The board to generate legal moves on.
/// @return All legal moves on the board.
[[maybe_unused]] [[nodiscard]] std::vector<Move> generate_legal_moves(const Board &board) noexcept;

/// @brief Returns all legal captures on the board.
/// @param board The board to generate legal captures on.
/// @return All legal captures on the board.
[[maybe_unused]] [[nodiscard]] std::vector<Move> generate_legal_captures(const Board &board) noexcept;
}// namespace movegen
