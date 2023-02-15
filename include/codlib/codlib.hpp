#pragma once

/// @brief The base namespace for all codlib functionality.
#include <codlib/base.hpp>

/// @brief The namespace for all move encoding and decoding functionality.
#include <codlib/move.hpp>

/// @brief The namespace for all board functionality.
#include <codlib/board.hpp>

/// @brief The namespace for move generation functionality.
#include <codlib/movegen.hpp>

/// @brief The namespace for all zobrist functionality.
#include <codlib/movegen.hpp>

namespace codlib {
/// @brief Initializes all codlib functionality.
/// @note This function must be called before any other codlib functionality is used.
[[maybe_unused]] void init() noexcept;
}// namespace codlib
