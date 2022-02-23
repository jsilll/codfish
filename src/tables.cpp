#include "tables.hpp"

#include "utils.hpp"
#include "magics.hpp"
#include "attacks.hpp"
#include <iostream>
#include <cstring>

U64 tables::SQUARE_BB[N_SQUARES];
U64 tables::ATTACKS_PAWN[N_SIDES][N_SQUARES];
U64 tables::ATTACKS_KNIGHT[N_SQUARES];
U64 tables::ATTACKS_KING[N_SQUARES];
U64 tables::ATTACKS_BISHOP[N_SQUARES][512];
U64 tables::ATTACKS_ROOK[N_SQUARES][4096];

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
        tables::ATTACKS_PAWN[WHITE][sq] = attacks::maskWhitePawnAnyAttacks(tables::SQUARE_BB[sq]);
        tables::ATTACKS_PAWN[BLACK][sq] = attacks::maskBlackPawnAnyAttacks(tables::SQUARE_BB[sq]);
    }

    for (int sq = A1; sq < N_SQUARES; sq++)
    {
        tables::ATTACKS_KNIGHT[sq] = attacks::maskKnightAttacks(tables::SQUARE_BB[sq]);
    }

    for (int sq = A1; sq < N_SQUARES; sq++)
    {
        tables::ATTACKS_KING[sq] = attacks::maskKingAttacks(tables::SQUARE_BB[sq]);
    }

    // Initialize Slider Piece Attack Tables
    for (int sq = A1; sq < N_SQUARES; sq++)
    {
        int occupancy_indices = 1 << tables::RELEVANT_BITS_COUNT_BISHOP[sq];
        for (int i = 0; i < occupancy_indices; i++)
        {
            U64 occupancy = utils::setOccupancy(i, RELEVANT_BITS_COUNT_BISHOP[sq], magics::MAGIC_TABLE_BISHOP[sq].mask);
            int magic = (occupancy * magics::MAGIC_TABLE_BISHOP[sq].magic) >> magics::MAGIC_TABLE_BISHOP[sq].shift;
            ATTACKS_BISHOP[sq][magic] = attacks::maskBishopAttacks(sq, occupancy);
        }
    }

    for (int sq = A1; sq < N_SQUARES; sq++)
    {
        int occupancy_indices = 1 << tables::RELEVANT_BITS_COUNT_ROOK[sq];
        for (int i = 0; i < occupancy_indices; i++)
        {
            U64 occupancy = utils::setOccupancy(i, RELEVANT_BITS_COUNT_ROOK[sq], magics::MAGIC_TABLE_ROOK[sq].mask);
            int magic = (occupancy * magics::MAGIC_TABLE_ROOK[sq].magic) >> magics::MAGIC_TABLE_ROOK[sq].shift;
            ATTACKS_ROOK[sq][magic] = attacks::maskRookAttacks(sq, occupancy);
        }
    }
}
