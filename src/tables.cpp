#include "tables.hpp"

#include "utils.hpp"
#include <iostream>

int tables::MS1BTABLE[256];

U64 tables::SQUARE_BB[N_SQUARES];

U64 tables::PAWN_ATTACKS[N_SIDES][N_SQUARES];
U64 tables::KNIGHT_ATTACKS[N_SQUARES];
U64 tables::KING_ATTACKS[N_SQUARES];

U64 whitePawnAnyAttacks(U64 wpawns);
U64 blackPawnAnyAttacks(U64 bpawns);
U64 knightAttacks(U64 knights);
U64 kingAttacks(U64 kings);

void tables::init()
{
    for (int sq = 0; sq < N_SQUARES; sq++)
    {
        tables::SQUARE_BB[sq] = ONE << sq;

        tables::PAWN_ATTACKS[WHITE][sq] = whitePawnAnyAttacks(tables::SQUARE_BB[sq]);
        tables::PAWN_ATTACKS[BLACK][sq] = blackPawnAnyAttacks(tables::SQUARE_BB[sq]);

        tables::KNIGHT_ATTACKS[sq] = knightAttacks(tables::SQUARE_BB[sq]);

        tables::KING_ATTACKS[sq] = kingAttacks(tables::SQUARE_BB[sq]);
    }

    for (int i = 0; i < 256; i++)
    {
        MS1BTABLE[i] = ((i > 127) ? 7 : (i > 63) ? 6
                                    : (i > 31)   ? 5
                                    : (i > 15)   ? 4
                                    : (i > 7)    ? 3
                                    : (i > 3)    ? 2
                                    : (i > 1)    ? 1
                                                 : 0);
    }
}

inline U64 wPawnEastAttacks(U64 wpawns) { return utils::noEaOne(wpawns); }
inline U64 wPawnWestAttacks(U64 wpawns) { return utils::noWeOne(wpawns); }

inline U64 bPawnEastAttacks(U64 bpawns) { return utils::soEaOne(bpawns); }
inline U64 bPawnWestAttacks(U64 bpawns) { return utils::soWeOne(bpawns); }

U64 whitePawnAnyAttacks(U64 wpawns)
{
    return wPawnEastAttacks(wpawns) | wPawnWestAttacks(wpawns);
}

U64 blackPawnAnyAttacks(U64 bpawns)
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

U64 knightAttacks(U64 knights) // TODO: understand code and use Directions Enum
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
    U64 attacks = utils::eastOne(kings) | utils::westOne(kings);
    kings |= attacks;
    attacks |= utils::nortOne(kings) | utils::soutOne(kings);
    return attacks;
}