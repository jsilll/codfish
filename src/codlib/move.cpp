#include <codlib/move.hpp>

#include <codlib/display.hpp>

/// @brief Returns the move in UCI format
/// @return The move in UCI format
std::string Move::get_uci() const noexcept {
    if (is_promotion()) {
        return SQUARE_NAMES[get_from_square()] + SQUARE_NAMES[get_to_square()] +
               PIECE_REPR[get_promoted_piece_type() + 6];
    } else {
        return SQUARE_NAMES[get_from_square()] + SQUARE_NAMES[get_to_square()];
    }
}
