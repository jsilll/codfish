#include <codchess/codchess.hpp>

#include "zobrist.hpp"
#include "movegen/magics.hpp"
#include "movegen/attacks.hpp"

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