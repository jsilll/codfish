#include "movegen.hpp"
#include "utils.hpp"
#include "tables.hpp"
#include "magics.hpp"
#include "board.hpp"

inline U64 getBishopAttacks(int sq, U64 occ)
{
    occ &= Magics::MAGIC_TABLE_BISHOP[sq].mask;
    occ *= Magics::MAGIC_TABLE_BISHOP[sq].magic;
    occ >>= Magics::MAGIC_TABLE_BISHOP[sq].shift;
    return Tables::ATTACKS_BISHOP[sq][occ];
}

inline U64 getRookAttacks(int sq, U64 occ)
{
    occ &= Magics::MAGIC_TABLE_ROOK[sq].mask;
    occ *= Magics::MAGIC_TABLE_ROOK[sq].magic;
    occ >>= Magics::MAGIC_TABLE_ROOK[sq].shift;
    return Tables::ATTACKS_ROOK[sq][occ];
}

void Movegen::generateMoves(Board &board)
{
}