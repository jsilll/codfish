#pragma once

#include "defs.hpp"
#include "tables.hpp"
#include "magics.hpp"

namespace movegen
{
    inline U64 getBishopAttacks(int sq, U64 occ)
    {
        occ &= tables::MAGIC_TABLE_BISHOP[sq].mask;
        occ *= tables::MAGIC_TABLE_BISHOP[sq].magic;
        occ >>= tables::MAGIC_TABLE_BISHOP[sq].shift;
        return tables::ATTACKS_BISHOP[sq][occ];
    }

    inline U64 getRookAttacks(int sq, U64 occ)
    {
        occ &= tables::MAGIC_TABLE_ROOK[sq].mask;
        occ *= tables::MAGIC_TABLE_ROOK[sq].magic;
        occ >>= tables::MAGIC_TABLE_ROOK[sq].shift;
        return tables::ATTACKS_ROOK[sq][occ];
    }
}