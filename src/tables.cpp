#include "tables.hpp"
#include "bitboard.hpp"
#include <iostream>

using bb::BLACK, bb::WHITE;
using bb::Color;
using bb::NORTH_WEST, bb::NORTH_EAST, bb::SOUTH_WEST, bb::SOUTH_EAST;
using bb::Square;
using bb::ZERO;

U64 tables::SQUARE_BB[64];
U64 tables::PAWN_ATTACKS[2][64];
U64 tables::KNIGHT_ATTACKS[64];
U64 tables::KING_ATTACKS[64];

U64 wPawnAnyAttacks(U64 wpawns);
U64 bPawnAnyAttacks(U64 bpawns);
U64 knightAttacks(U64 knights);
U64 kingAttacks(U64 kings);

void tables::init()
{
    for (int sq = 0; sq < 64; sq++)
    {
        tables::SQUARE_BB[sq] = 1ULL << sq;

        tables::PAWN_ATTACKS[WHITE][sq] = wPawnAnyAttacks(tables::SQUARE_BB[sq]);
        tables::PAWN_ATTACKS[BLACK][sq] = bPawnAnyAttacks(tables::SQUARE_BB[sq]);

        tables::KNIGHT_ATTACKS[sq] = knightAttacks(tables::SQUARE_BB[sq]);

        tables::KING_ATTACKS[sq] = kingAttacks(tables::SQUARE_BB[sq]);
    }
}

inline U64 wPawnEastAttacks(U64 wpawns) { return bb::noEaOne(wpawns); }
inline U64 wPawnWestAttacks(U64 wpawns) { return bb::noWeOne(wpawns); }

inline U64 bPawnEastAttacks(U64 bpawns) { return bb::soEaOne(bpawns); }
inline U64 bPawnWestAttacks(U64 bpawns) { return bb::soWeOne(bpawns); }

U64 wPawnAnyAttacks(U64 wpawns)
{
    return wPawnEastAttacks(wpawns) | wPawnWestAttacks(wpawns);
}

U64 bPawnAnyAttacks(U64 bpawns)
{
    return bPawnEastAttacks(bpawns) | bPawnWestAttacks(bpawns);
}

// TODO: Pawn pushes
// TODO: Pawn en passant

// TODO: should be used in move gen BE USED
// U64 wPawnDblAttacks(U64 wpawns)
// {
//     return wPawnEastAttacks(wpawns) & wPawnWestAttacks(wpawns);
// }

// U64 wPawnSingleAttacks(U64 wpawns)
// {
//     return wPawnEastAttacks(wpawns) ^ wPawnWestAttacks(wpawns);
// }

U64 knightAttacks(U64 knights)
{
    const U64 CLEAR_FILE_HG = 0x3f3f3f3f3f3f3f3f;
    const U64 CLEAR_FILE_AB = 0xfcfcfcfcfcfcfcfc;
    U64 l1 = (knights >> 1) & tables::CLEAR_FILE[7];
    U64 l2 = (knights >> 2) & CLEAR_FILE_HG;
    U64 r1 = (knights << 1) & tables::CLEAR_FILE[0];
    U64 r2 = (knights << 2) & CLEAR_FILE_AB;
    U64 h1 = l1 | r1;
    U64 h2 = l2 | r2;
    return (h1 << 16) | (h1 >> 16) | (h2 << 8) | (h2 >> 8);
}

U64 kingAttacks(U64 kings)
{
    U64 attacks = bb::eastOne(kings) | bb::westOne(kings);
    kings |= attacks;
    attacks |= bb::nortOne(kings) | bb::soutOne(kings);
    return attacks;
}