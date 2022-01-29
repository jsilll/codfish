#include "tables.hpp"

#include "utils.hpp"
#include <iostream>

int tables::MS1BTABLE[256];

U64 tables::SQUARE_BB[N_SQUARES];

U64 tables::PAWN_ATTACKS[N_SIDES][N_SQUARES];
U64 tables::KNIGHT_ATTACKS[N_SQUARES];
U64 tables::KING_ATTACKS[N_SQUARES];

// U64 tables::RAY_ATTACKS[N_SQUARES][N_DIRECTIONS];

U64 whitePawnAnyAttacks(U64 wpawns);
U64 blackPawnAnyAttacks(U64 bpawns);
U64 knightAttacks(U64 knights);
U64 kingAttacks(U64 kings);

U64 bishopAttacks(int sq);
U64 rookAttacks(int sq);
U64 bishopAttacks(int sq, U64 block);
U64 rookAttacks(int sq, U64 block);

inline U64 whitePawnEastAttacks(U64 wpawns) { return utils::noEaOne(wpawns); }
inline U64 whitePawnWestAttacks(U64 wpawns) { return utils::noWeOne(wpawns); }
inline U64 blackPawnEastAttacks(U64 bpawns) { return utils::soEaOne(bpawns); }
inline U64 blackPawnWestAttacks(U64 bpawns) { return utils::soWeOne(bpawns); }

void tables::init()
{
    // TODO: maybe multiple loops for each table?
    for (int sq = A1; sq < N_SQUARES; sq++)
    {
        tables::SQUARE_BB[sq] = ONE << sq;
        tables::PAWN_ATTACKS[WHITE][sq] = whitePawnAnyAttacks(tables::SQUARE_BB[sq]);
        tables::PAWN_ATTACKS[BLACK][sq] = blackPawnAnyAttacks(tables::SQUARE_BB[sq]);
        tables::KNIGHT_ATTACKS[sq] = knightAttacks(tables::SQUARE_BB[sq]);
        tables::KING_ATTACKS[sq] = kingAttacks(tables::SQUARE_BB[sq]);
    }

    for (int i = 0; i < 256; i++)
    {
        // clang-format off
        MS1BTABLE[i] = ((i > 127) ? 7 : (i > 63) ? 6 : (i > 31)   ? 5 : (i > 15)   ? 4 : (i > 7)    ? 3 : (i > 3)    ? 2 : (i > 1)    ? 1 : 0);
        // clang-format on
    }

    // Initializing RAY_ATTACKS array
    // U64 nort = 0x0101010101010100;
    // for (int sq = 0; sq < 64; sq++, nort <<= 1)
    // {
    //     tables::RAY_ATTACKS[sq][NORTH] = nort;
    // }

    // U64 noea = 0x8040201008040200;
    // for (int f = 0; f < 8; f++, noea = utils::eastOne(noea))
    // {
    //     U64 ne = noea;
    //     for (int r8 = 0; r8 < 8 * 8; r8 += 8, ne <<= 8)
    //         RAY_ATTACKS[r8 + f][NORTH_EAST] = ne;
    // }

    // TODO: Debug this block of code
    // U64 nowe = 0x102040810204000;
    // for (int f = 0; f < 8; f++, noea = utils::westOne(nowe))
    // {
    //     U64 no = noea;
    //     for (int r8 = 0; r8 < 8 * 8; r8 += 8, no <<= 8)
    //     {
    //         RAY_ATTACKS[r8 + f][NORTH_WEST] = no;
    //         utils::printBB(RAY_ATTACKS[r8 + f][NORTH_WEST]);
    //     }
    // }
}

U64 whitePawnAnyAttacks(U64 wpawns)
{
    return whitePawnEastAttacks(wpawns) | whitePawnWestAttacks(wpawns);
}

U64 blackPawnAnyAttacks(U64 bpawns)
{
    return blackPawnEastAttacks(bpawns) | blackPawnWestAttacks(bpawns);
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

U64 bishopAttacks(int sq)
{
    U64 attacks = ZERO;
    int rank = utils::getRank(sq);
    int file = utils::getFile(sq);
    for (int r = rank + 1, f = file + 1; r < 7 && f < 7; r++, f++)
        attacks |= (ONE << utils::getSquare(r, f));
    for (int r = rank + 1, f = file - 1; r < 7 && f > 0; r++, f--)
        attacks |= (ONE << utils::getSquare(r, f));
    for (int r = rank - 1, f = file + 1; r > 0 && f < 7; r--, f++)
        attacks |= (ONE << utils::getSquare(r, f));
    for (int r = rank - 1, f = file - 1; r > 0 && f > 0; r--, f--)
        attacks |= (ONE << utils::getSquare(r, f));
    return attacks;
}

U64 rookAttacks(int sq)
{
    U64 attacks = ZERO;
    int rank = utils::getRank(sq);
    int file = utils::getFile(sq);
    for (int r = rank + 1; r < 7; r++)
        attacks |= (ONE << utils::getSquare(r, file));
    for (int r = rank - 1; r > 0; r--)
        attacks |= (ONE << utils::getSquare(r, file));
    for (int f = file + 1; f < 7; f++)
        attacks |= (ONE << utils::getSquare(rank, f));
    for (int f = file - 1; f > 0; f--)
        attacks |= (ONE << utils::getSquare(rank, f));
    return attacks;
}

U64 bishopAttacks(int sq, U64 block)
{
    U64 attacks = ZERO;
    int rank = utils::getRank(sq);
    int file = utils::getFile(sq);
    for (int r = rank + 1, f = file + 1; r < 8 && f < 8; r++, f++)
    {
        attacks |= (ONE << utils::getSquare(r, f));
        if ((ONE << utils::getSquare(r, f)) & block)
            break;
    }
    for (int r = rank + 1, f = file - 1; r < 8 && f >= 0; r++, f--)
    {
        attacks |= (ONE << utils::getSquare(r, f));
        if ((ONE << utils::getSquare(r, f)) & block)
            break;
    }
    for (int r = rank - 1, f = file + 1; r >= 0 && f < 8; r--, f++)
    {
        attacks |= (ONE << utils::getSquare(r, f));
        if ((ONE << utils::getSquare(r, f)) & block)
            break;
    }
    for (int r = rank - 1, f = file - 1; r >= 0 && f >= 0; r--, f--)
    {
        attacks |= (ONE << utils::getSquare(r, f));
        if ((ONE << utils::getSquare(r, f)) & block)
            break;
    }
    return attacks;
}

U64 rookAttacks(int sq, U64 block)
{
    U64 attacks = ZERO;
    int rank = utils::getRank(sq);
    int file = utils::getFile(sq);
    for (int r = rank + 1; r < 8; r++)
    {
        attacks |= (ONE << utils::getSquare(r, file));
        if ((ONE << utils::getSquare(r, file)) & block)
            break;
    }
    for (int r = rank - 1; r >= 0; r--)
    {
        attacks |= (ONE << utils::getSquare(r, file));
        if ((ONE << utils::getSquare(r, file)) & block)
            break;
    }
    for (int f = file + 1; f < 8; f++)
    {
        attacks |= (ONE << utils::getSquare(rank, f));
        if ((ONE << utils::getSquare(rank, f)) & block)
            break;
    }
    for (int f = file - 1; f >= 0; f--)
    {
        attacks |= (ONE << utils::getSquare(rank, f));
        if ((ONE << utils::getSquare(rank, f)) & block)
            break;
    }
    return attacks;
}