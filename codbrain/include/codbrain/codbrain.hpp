#pragma once

#include <codbrain/brain.hpp>

namespace codbrain {
/// @brief Initializes the library
void Init() noexcept {
    eval::Init();
    codchess::Init();
}
} // namespace codbrain