#include "tables.hpp"

#include "utils.hpp"
#include <iostream>
#include <cstring>

U64 tables::SQUARE_BB[N_SQUARES];

U64 tables::PAWN_ATTACKS[N_SIDES][N_SQUARES];
U64 tables::KNIGHT_ATTACKS[N_SQUARES];
U64 tables::KING_ATTACKS[N_SQUARES];

inline U64 whitePawnEastAttacks(U64 wpawns) { return utils::noEaOne(wpawns); }
inline U64 whitePawnWestAttacks(U64 wpawns) { return utils::noWeOne(wpawns); }
inline U64 blackPawnEastAttacks(U64 bpawns) { return utils::soEaOne(bpawns); }
inline U64 blackPawnWestAttacks(U64 bpawns) { return utils::soWeOne(bpawns); }
U64 maskWhitePawnAnyAttacks(U64 wpawns);
U64 maskBlackPawnAnyAttacks(U64 bpawns);
U64 maskKnightAttacks(U64 knights);
U64 maskKingAttacks(U64 kings);

void tables::init()
{
    for (int sq = A1; sq < N_SQUARES; sq++)
    {
        tables::SQUARE_BB[sq] = ONE << sq;
    }

    for (int sq = A1; sq < N_SQUARES; sq++)
    {
        tables::PAWN_ATTACKS[WHITE][sq] = maskWhitePawnAnyAttacks(tables::SQUARE_BB[sq]);
        tables::PAWN_ATTACKS[BLACK][sq] = maskBlackPawnAnyAttacks(tables::SQUARE_BB[sq]);
    }

    for (int sq = A1; sq < N_SQUARES; sq++)
    {
        tables::KNIGHT_ATTACKS[sq] = maskKnightAttacks(tables::SQUARE_BB[sq]);
    }

    for (int sq = A1; sq < N_SQUARES; sq++)
    {
        tables::KING_ATTACKS[sq] = maskKingAttacks(tables::SQUARE_BB[sq]);
    }
}

U64 maskWhitePawnAnyAttacks(U64 wpawns)
{
    return whitePawnEastAttacks(wpawns) | whitePawnWestAttacks(wpawns);
}

U64 maskBlackPawnAnyAttacks(U64 bpawns)
{
    return blackPawnEastAttacks(bpawns) | blackPawnWestAttacks(bpawns);
}

U64 maskKnightAttacks(U64 knights)
{
    static const U64 CLEAR_FILE_HG = 0x3f3f3f3f3f3f3f3f;
    static const U64 CLEAR_FILE_AB = 0xfcfcfcfcfcfcfcfc;

    U64 l1 = (knights >> 1) & tables::CLEAR_FILE[7];
    U64 r1 = (knights << 1) & tables::CLEAR_FILE[0];
    U64 h1 = l1 | r1;

    U64 l2 = (knights >> 2) & CLEAR_FILE_HG;
    U64 r2 = (knights << 2) & CLEAR_FILE_AB;
    U64 h2 = l2 | r2;

    return (h1 << 16) | (h1 >> 16) | (h2 << 8) | (h2 >> 8);
}

U64 maskKingAttacks(U64 kings)
{
    U64 attacks = utils::eastOne(kings) | utils::westOne(kings);
    kings |= attacks;
    attacks |= utils::nortOne(kings) | utils::soutOne(kings);
    return attacks;
}