#pragma once

#include <codchess/board.hpp>

namespace codchess::perft {
/// @brief Perft function for testing move generation
/// @param board The board to move generation on
/// @param depth The depth to search to
/// @return The number of nodes found
int Perft(Board &board, int depth) noexcept;
}   // namespace codchess::perft