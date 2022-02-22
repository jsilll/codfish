#include "tables.hpp"

#include "utils.hpp"
#include "magics.hpp"
#include <iostream>
#include <cstring>

U64 tables::SQUARE_BB[N_SQUARES];

U64 tables::ATTACKS_PAWN[N_SIDES][N_SQUARES];
U64 tables::ATTACKS_KNIGHT[N_SQUARES];
U64 tables::ATTACKS_KING[N_SQUARES];

U64 tables::maskBishopAttacks(int sq, U64 block);
U64 tables::maskRookAttacks(int sq, U64 block);
U64 tables::maskRookAttackRays(int sq);
U64 tables::maskBishopAttackRays(int sq);

U64 tables::ATTACKS_BISHOP[N_SQUARES][512];
U64 tables::ATTACKS_ROOK[N_SQUARES][4096];

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
    // Misc. Tables
    for (int sq = A1; sq < N_SQUARES; sq++)
    {
        tables::SQUARE_BB[sq] = ONE << sq;
    }

    // Initializing Leaper Piece Attack Tables

    for (int sq = A1; sq < N_SQUARES; sq++)
    {
        tables::ATTACKS_PAWN[WHITE][sq] = maskWhitePawnAnyAttacks(tables::SQUARE_BB[sq]);
        tables::ATTACKS_PAWN[BLACK][sq] = maskBlackPawnAnyAttacks(tables::SQUARE_BB[sq]);
    }

    for (int sq = A1; sq < N_SQUARES; sq++)
    {
        tables::ATTACKS_KNIGHT[sq] = maskKnightAttacks(tables::SQUARE_BB[sq]);
    }

    for (int sq = A1; sq < N_SQUARES; sq++)
    {
        tables::ATTACKS_KING[sq] = maskKingAttacks(tables::SQUARE_BB[sq]);
    }

    // Initialize slider piece Attack Tables
    for (int sq = A1; sq < N_SQUARES; sq++)
    {
        int occupancy_indices = 1 << tables::RELEVANT_BITS_COUNT_BISHOP[sq];
        for (int i = 0; i < occupancy_indices; i++)
        {
            U64 occupancy = utils::setOccupancy(i, RELEVANT_BITS_COUNT_BISHOP[sq], magics::MAGIC_TABLE_BISHOP[sq].mask);
            int magic_index = (occupancy * magics::MAGIC_TABLE_BISHOP[sq].magic) >> magics::MAGIC_TABLE_BISHOP[sq].shift;
            ATTACKS_BISHOP[sq][magic_index] = maskBishopAttacks(sq, occupancy);
        }
    }

    for (int sq = A1; sq < N_SQUARES; sq++)
    {
        int occupancy_indices = 1 << tables::RELEVANT_BITS_COUNT_ROOK[sq];
        for (int i = 0; i < occupancy_indices; i++)
        {
            U64 occupancy = utils::setOccupancy(i, RELEVANT_BITS_COUNT_ROOK[sq], magics::MAGIC_TABLE_ROOK[sq].mask);
            int magic_index = (occupancy * magics::MAGIC_TABLE_ROOK[sq].magic) >> magics::MAGIC_TABLE_ROOK[sq].shift;
            ATTACKS_ROOK[sq][magic_index] = maskRookAttacks(sq, occupancy);
        }
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

    U64 l1 = (knights >> 1) & tables::MASK_CLEAR_FILE[7];
    U64 r1 = (knights << 1) & tables::MASK_CLEAR_FILE[0];
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

U64 tables::maskBishopAttackRays(int sq)
{
    U64 attacks = ZERO;
    int rank = utils::getRank(sq);
    int file = utils::getFile(sq);
    for (int r = rank + 1, f = file + 1; r < 7 && f < 7; r++, f++)
    {
        attacks |= (ONE << utils::getSquare(r, f));
    }
    for (int r = rank + 1, f = file - 1; r < 7 && f > 0; r++, f--)
    {
        attacks |= (ONE << utils::getSquare(r, f));
    }
    for (int r = rank - 1, f = file + 1; r > 0 && f < 7; r--, f++)
    {
        attacks |= (ONE << utils::getSquare(r, f));
    }
    for (int r = rank - 1, f = file - 1; r > 0 && f > 0; r--, f--)
    {
        attacks |= (ONE << utils::getSquare(r, f));
    }
    return attacks;
}

U64 tables::maskRookAttackRays(int sq)
{
    U64 attacks = ZERO;
    int rank = utils::getRank(sq);
    int file = utils::getFile(sq);
    for (int r = rank + 1; r < 7; r++)
    {
        attacks |= (ONE << utils::getSquare(r, file));
    }
    for (int r = rank - 1; r > 0; r--)
    {
        attacks |= (ONE << utils::getSquare(r, file));
    }
    for (int f = file + 1; f < 7; f++)
    {
        attacks |= (ONE << utils::getSquare(rank, f));
    }
    for (int f = file - 1; f > 0; f--)
    {
        attacks |= (ONE << utils::getSquare(rank, f));
    }
    return attacks;
}

U64 tables::maskBishopAttacks(int sq, U64 block)
{
    U64 attacks = ZERO;
    int rank = utils::getRank(sq);
    int file = utils::getFile(sq);
    for (int r = rank + 1, f = file + 1; r < 8 && f < 8; r++, f++)
    {
        attacks |= (ONE << utils::getSquare(r, f));
        if ((ONE << utils::getSquare(r, f)) & block)
        {
            break;
        }
    }
    for (int r = rank + 1, f = file - 1; r < 8 && f >= 0; r++, f--)
    {
        attacks |= (ONE << utils::getSquare(r, f));
        if ((ONE << utils::getSquare(r, f)) & block)
        {
            break;
        }
    }
    for (int r = rank - 1, f = file + 1; r >= 0 && f < 8; r--, f++)
    {
        attacks |= (ONE << utils::getSquare(r, f));
        if ((ONE << utils::getSquare(r, f)) & block)
        {
            break;
        }
    }
    for (int r = rank - 1, f = file - 1; r >= 0 && f >= 0; r--, f--)
    {
        attacks |= (ONE << utils::getSquare(r, f));
        if ((ONE << utils::getSquare(r, f)) & block)
        {
            break;
        }
    }
    return attacks;
}

U64 tables::maskRookAttacks(int sq, U64 block)
{
    U64 attacks = ZERO;
    int rank = utils::getRank(sq);
    int file = utils::getFile(sq);
    for (int r = rank + 1; r < 8; r++)
    {
        attacks |= (ONE << utils::getSquare(r, file));
        if ((ONE << utils::getSquare(r, file)) & block)
        {
            break;
        }
    }
    for (int r = rank - 1; r >= 0; r--)
    {
        attacks |= (ONE << utils::getSquare(r, file));
        if ((ONE << utils::getSquare(r, file)) & block)
        {
            break;
        }
    }
    for (int f = file + 1; f < 8; f++)
    {
        attacks |= (ONE << utils::getSquare(rank, f));
        if ((ONE << utils::getSquare(rank, f)) & block)
        {
            break;
        }
    }
    for (int f = file - 1; f >= 0; f--)
    {
        attacks |= (ONE << utils::getSquare(rank, f));
        if ((ONE << utils::getSquare(rank, f)) & block)
        {
            break;
        }
    }
    return attacks;
}