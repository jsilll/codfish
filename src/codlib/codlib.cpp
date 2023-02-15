#include <codlib/codlib.hpp>

#include <codlib/movegen/attacks.hpp>
#include <codlib/movegen/magics.hpp>
#include <codlib/zobrist.hpp>

[[maybe_unused]] void init() noexcept {
    magics::init();
    zobrist::init();
    attacks::init();
}