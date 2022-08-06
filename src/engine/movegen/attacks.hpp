#pragma once

#include <engine/constants.hpp>

namespace attacks
{
    // Pawn Pushes
    u64 mask_white_pawn_single_pushes(u64 wpawns, u64 empty);
    u64 mask_black_pawn_single_pushes(u64 bpawns, u64 empty);
    u64 mask_white_pawn_double_pushes(u64 wpawns, u64 empty);
    u64 mask_black_pawn_double_pushes(u64 bpawns, u64 empty);

    // Bishop and Rook Attack Rays
    u64 mask_rook_attack_rays(int sq);
    u64 mask_bishop_attack_rays(int sq);

    // Attacks
    u64 mask_white_pawn_any_attacks(u64 wpawns);
    u64 mask_black_pawn_any_attacks(u64 bpawns);
    u64 mask_knight_attacks(u64 knights);
    u64 mask_king_attacks(u64 kings);
    u64 mask_bishop_attacks(int sq, u64 block);
    u64 mask_rook_attacks(int sq, u64 block);

} // namespace attacks