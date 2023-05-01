#include <codchess/codchess.hpp>

#include "attacks.hpp"
#include "magics.hpp"
#include "zobrist.hpp"

namespace codchess {
/// @brief Initializes all codchess functionality.
/// @note This function must be called before any other codchess functionality
/// is used.
[[maybe_unused]] void
Init() noexcept {
    magics::Init();
    zobrist::Init();
    attacks::Init();
}
}   // namespace codchess