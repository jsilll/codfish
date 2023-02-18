#pragma once

/// @brief The base namespace for all codlib functionality.
#include <codchess/base.hpp>

/// @brief The namespace for all move encoding and decoding functionality.
#include <codchess/move.hpp>

/// @brief The namespace for all board functionality.
#include <codchess/board.hpp>

/// @brief The namespace for move generation functionality.
#include <codchess/movegen.hpp>

/// @brief The namespace for all zobrist functionality.
#include <codchess/movegen.hpp>

namespace codlib {

    /// @brief Initializes all codlib functionality.
    /// @note This function must be called before any other codlib functionality is used.
    [[maybe_unused]] void Init() noexcept;

}// namespace codlib
