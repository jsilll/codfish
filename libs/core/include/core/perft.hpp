#pragma once

#include <core/codchess.hpp>

namespace codfish::core::perft {
/// @brief Function for testing move generation
/// @param board The board to move generation on
/// @param depth The depth to search to
/// @return The number of nodes found
/// @note This function uses the unmake method
std::uint64_t PerftUnmake(const Board &board, std::uint32_t depth) noexcept;

/// @brief Function for testing move generation
/// @param board The board to move generation on
/// @param depth The depth to search to
/// @return The number of nodes found
/// @note This function uses the copy method
std::uint64_t PerftCopy(const Board &board, std::uint32_t depth) noexcept;
}   // namespace codfish::core::perft