#include "tables.hpp"

#include "utils.hpp"
#include "magics.hpp"
#include "attacks.hpp"
#include <iostream>
#include <cstring>

U64 Tables::SQUARE_BB[N_SQUARES];
U64 Tables::ATTACKS_PAWN[BOTH][N_SQUARES];
U64 Tables::ATTACKS_KNIGHT[N_SQUARES];
U64 Tables::ATTACKS_KING[N_SQUARES];
U64 Tables::ATTACKS_BISHOP[N_SQUARES][512];
U64 Tables::ATTACKS_ROOK[N_SQUARES][4096];

void Tables::init()
{
    // Misc. Tables
    for (int sq = A1; sq < N_SQUARES; sq++)
    {
        Tables::SQUARE_BB[sq] = ONE << sq;
    }

    // Initializing Leaper Piece Attack Tables
    for (int sq = A1; sq < N_SQUARES; sq++)
    {
        Tables::ATTACKS_PAWN[WHITE][sq] = Attacks::maskWhitePawnAnyAttacks(Tables::SQUARE_BB[sq]);
        Tables::ATTACKS_PAWN[BLACK][sq] = Attacks::maskBlackPawnAnyAttacks(Tables::SQUARE_BB[sq]);
    }

    for (int sq = A1; sq < N_SQUARES; sq++)
    {
        Tables::ATTACKS_KNIGHT[sq] = Attacks::maskKnightAttacks(Tables::SQUARE_BB[sq]);
    }

    for (int sq = A1; sq < N_SQUARES; sq++)
    {
        Tables::ATTACKS_KING[sq] = Attacks::maskKingAttacks(Tables::SQUARE_BB[sq]);
    }

    // Initialize Slider Piece Attack Tables
    for (int sq = A1; sq < N_SQUARES; sq++)
    {
        int occupancy_indices = 1 << Tables::RELEVANT_BITS_COUNT_BISHOP[sq];
        for (int i = 0; i < occupancy_indices; i++)
        {
            U64 occupancy = Utils::setOccupancy(i, RELEVANT_BITS_COUNT_BISHOP[sq], Magics::MAGIC_TABLE_BISHOP[sq].mask);
            int magic = (occupancy * Magics::MAGIC_TABLE_BISHOP[sq].magic) >> Magics::MAGIC_TABLE_BISHOP[sq].shift;
            ATTACKS_BISHOP[sq][magic] = Attacks::maskBishopAttacks(sq, occupancy);
        }
    }

    for (int sq = A1; sq < N_SQUARES; sq++)
    {
        int occupancy_indices = 1 << Tables::RELEVANT_BITS_COUNT_ROOK[sq];
        for (int i = 0; i < occupancy_indices; i++)
        {
            U64 occupancy = Utils::setOccupancy(i, RELEVANT_BITS_COUNT_ROOK[sq], Magics::MAGIC_TABLE_ROOK[sq].mask);
            int magic = (occupancy * Magics::MAGIC_TABLE_ROOK[sq].magic) >> Magics::MAGIC_TABLE_ROOK[sq].shift;
            ATTACKS_ROOK[sq][magic] = Attacks::maskRookAttacks(sq, occupancy);
        }
    }
}
