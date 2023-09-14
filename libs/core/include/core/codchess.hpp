#pragma once

#include <core/base.hpp>
#include <core/move.hpp>
#include <core/board.hpp>
#include <core/movegen.hpp>

namespace codfish::core {
/// @brief Initializes all codchess functionality.
/// @note This function must be called before any other codchess functionality
/// @note is used.
[[maybe_unused]] void Init() noexcept;
}   // namespace codfish::core
