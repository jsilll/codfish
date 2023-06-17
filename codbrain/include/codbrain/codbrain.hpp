#pragma once

#include <codchess/codchess.hpp>

#include <codbrain/brain.hpp>

namespace codbrain {
/// @brief Initializes the library
inline void Init() noexcept {
    eval::Init();
    codchess::Init();
}
} // namespace codbrain