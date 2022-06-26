#pragma once

#include "../defs.hpp"

namespace attacks
{
    U64 maskWhitePawnSinglePushes(U64 wpawns, U64 empty);
    U64 maskBlackPawnSinglePushes(U64 bpawns, U64 empty);
    U64 maskWhitePawnDoublePushes(U64 wpawns, U64 empty);
    U64 maskBlackPawnDoublePushes(U64 bpawns, U64 empty);

    U64 maskRookAttackRays(int sq);
    U64 maskBishopAttackRays(int sq);

    U64 maskWhitePawnAnyAttacks(U64 wpawns);
    U64 maskBlackPawnAnyAttacks(U64 bpawns);
    U64 maskKnightAttacks(U64 knights);
    U64 maskKingAttacks(U64 kings);
    U64 maskBishopAttacks(int sq, U64 block);
    U64 maskRookAttacks(int sq, U64 block);

} // namespace attacks