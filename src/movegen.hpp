#pragma once

#include "defs.hpp"
#include "tables.hpp"
#include "magics.hpp"

namespace movegen
{
    inline U64 getBishopAttacks(int sq, U64 occupancy)
    {
        occupancy &= tables::RAYS_BISHOP[sq];
        occupancy *= magics::BISHOP_MAGICS[sq];
        occupancy >>= 64 - tables::RELEVANT_BITS_COUNT_BISHOP[sq];
        return tables::ATTACKS_BISHOP[sq][(int)occupancy];
    }

    inline U64 getRookAttacks(int sq, U64 occupancy)
    {
        occupancy &= tables::RAYS_ROOK[sq];
        occupancy *= magics::ROOK_MAGICS[sq];
        occupancy >>= 64 - tables::RELEVANT_BITS_COUNT_ROOK[sq];
        return tables::ATTACKS_ROOK[sq][(int)occupancy];
    }
}