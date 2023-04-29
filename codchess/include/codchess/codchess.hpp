#pragma once

#include <codchess/base.hpp>

#include <codchess/move.hpp>

#include <codchess/board.hpp>

#include <codchess/movegen.hpp>

namespace codchess {
/// @brief Initializes all codchess functionality.
/// @note This function must be called before any other codchess functionality
/// @note is used.
[[maybe_unused]] void Init() noexcept;
}   // namespace codchess
