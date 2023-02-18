#include <codchess/codlib.hpp>

#include <codchess/zobrist.hpp>
#include <codchess/movegen/attacks.hpp>
#include <codchess/movegen/magics.hpp>

namespace codlib {

    /// @brief Initializes all codlib functionality.
    /// @note This function must be called before any other codlib functionality is used.
    [[maybe_unused]] void Init() noexcept {
        magics::Init();
        zobrist::Init();
        attacks::Init();
    }

}// namespace codlib