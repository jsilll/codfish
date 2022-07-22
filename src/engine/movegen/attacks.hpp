#pragma once

#include <engine/constants.hpp>

namespace attacks
{
    // Pawn Pushes
    U64 mask_white_pawn_single_pushes(U64 wpawns, U64 empty);
    U64 mask_black_pawn_single_pushes(U64 bpawns, U64 empty);
    U64 mask_white_pawn_double_pushes(U64 wpawns, U64 empty);
    U64 mask_black_pawn_double_pushes(U64 bpawns, U64 empty);

    // Bishop and Rook Attack Rays
    U64 mask_rook_attack_rays(int sq);
    U64 mask_bishop_attack_rays(int sq);

    // Attacks
    U64 mask_white_pawn_any_attacks(U64 wpawns);
    U64 mask_black_pawn_any_attacks(U64 bpawns);
    U64 mask_knight_attacks(U64 knights);
    U64 mask_king_attacks(U64 kings);
    U64 mask_bishop_attacks(int sq, U64 block);
    U64 mask_rook_attacks(int sq, U64 block);

} // namespace attacks