#include <codlib/codlib.hpp>

#include "codlib/zobrist.hpp"
#include <codlib/movegen/attacks.hpp>
#include <codlib/movegen/magics.hpp>

namespace codlib {
    /// @brief Initializes all codlib functionality.
    /// @note This function must be called before any other codlib functionality is used.
    [[maybe_unused]] void init() noexcept {
        magics::init();
        zobrist::init();
        attacks::init();
    }
}// namespace codlib