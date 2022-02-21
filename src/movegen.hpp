#pragma once

#include "defs.hpp"
#include "tables.hpp"
#include "magics.hpp"

namespace movegen
{
    inline U64 getBishopAttacks(int sq, U64 occupancy)
    {
        occupancy &= tables::MAGIC_TABLE_BISHOP[sq].mask;
        occupancy *= tables::MAGIC_TABLE_BISHOP[sq].magic;
        occupancy >>= tables::MAGIC_TABLE_BISHOP[sq].shift;
        return tables::ATTACKS_BISHOP[sq][occupancy];
    }

    inline U64 getRookAttacks(int sq, U64 occupancy)
    {
        occupancy &= tables::MAGIC_TABLE_ROOK[sq].mask;
        occupancy *= tables::MAGIC_TABLE_ROOK[sq].magic;
        occupancy >>= tables::MAGIC_TABLE_ROOK[sq].shift;
        return tables::ATTACKS_ROOK[sq][occupancy];
    }
}