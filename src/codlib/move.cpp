#include <codlib/move.hpp>

#include <codlib/display.hpp>

/// @brief Returns the move in UCI format
/// @return The move in UCI format
std::string Move::Uci() const noexcept {
    if (IsPromotion()) {
        return SQUARE_DISPLAY[FromSquare()] + SQUARE_DISPLAY[ToSquare()] + PIECE_DISPLAY[PromotedPiece() + 6];
    } else {
        return SQUARE_DISPLAY[FromSquare()] + SQUARE_DISPLAY[ToSquare()];
    }
}
