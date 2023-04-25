#include <codchess/codchess.hpp>

#include <codchess/movegen/attacks.hpp>
#include <codchess/movegen/magics.hpp>
#include <codchess/zobrist.hpp>

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