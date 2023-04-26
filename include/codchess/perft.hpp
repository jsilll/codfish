#pragma once

#include <codchess/board.hpp>

namespace codchess::perft {
/// @brief Perft function for testing move generation
/// @param board The board to move generation on
/// @param depth The depth to search to
/// @return The number of nodes found
std::uint32_t Perft(Board &board, std::uint32_t depth) noexcept;
}   // namespace codchess::perft