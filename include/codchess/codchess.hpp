#pragma once

// The base namespace for all  functionality.
#include <codchess/base.hpp>

// The namespace for all move encoding and decoding functionality.
#include <codchess/move.hpp>

// The namespace for all board functionality.
#include <codchess/board.hpp>

// The namespace for move generation functionality.
#include <codchess/movegen.hpp>

// The namespace for all zobrist functionality.
#include <codchess/movegen.hpp>

namespace codchess {
/// @brief Initializes all codchess functionality.
/// @note This function must be called before any other codchess functionality
/// @note is used.
[[maybe_unused]] void Init() noexcept;
}   // namespace codchess
