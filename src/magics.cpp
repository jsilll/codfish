#include "magics.hpp"

#include "utils.hpp"
#include "tables.hpp"
#include <iostream>
#include <cstring>

unsigned int generateRandomNumberU32()
{
    // XOR Shift 32 algorithm
    static unsigned int state = 1804289383;
    unsigned int number = state;
    number ^= number << 13;
    number ^= number >> 17;
    number ^= number << 5;
    state = number;
    return number;
}

U64 generateRandomNumberU64()
{
    U64 n1 = ((U64)generateRandomNumberU32()) & 0xFFFF;
    U64 n2 = ((U64)generateRandomNumberU32()) & 0xFFFF;
    U64 n3 = ((U64)generateRandomNumberU32()) & 0xFFFF;
    U64 n4 = ((U64)generateRandomNumberU32()) & 0xFFFF;
    return n1 | (n2 << 16) | (n3 << 32) | (n4 << 48);
}

inline U64 getMagicNumberCandidate()
{
    return generateRandomNumberU64() & generateRandomNumberU64() & generateRandomNumberU64();
}

U64 generateMagicNumber(int sq, int relevant_bits, U64 (*maskAttacksFun)(int), U64 (*maskAttacksOccFun)(int, U64))
{
    int occupancy_indices = 1 << relevant_bits;
    U64 attack_mask = maskAttacksFun(sq);
    U64 occupancies[4096], attacks[4096], used_attacks[4096];

    for (int i = 0; i < occupancy_indices; i++)
    {
        occupancies[i] = utils::setOccupancy(i, relevant_bits, attack_mask);
        attacks[i] = maskAttacksOccFun(sq, occupancies[i]);
    }

    for (int max_tries = 0; max_tries < 99999999; max_tries++)
    {
        U64 candidate = getMagicNumberCandidate();

        if (utils::bitCount((attack_mask * candidate) & 0xFF00000000000000) < 6)
        {
            continue;
        }

        memset(used_attacks, ZERO, sizeof(used_attacks));

        int fail = 0;
        for (int index = 0; !fail && index < occupancy_indices; index++)
        {
            int magic_index = (int)((occupancies[index] * candidate) >> (64 - relevant_bits));
            if (used_attacks[magic_index] == ZERO)
            {
                used_attacks[magic_index] = attacks[index];
            }
            else // alternatively, else if (used_attacks[magic_index] != attacks[index])
            {
                fail = 1;
                break;
            }
        }

        if (!fail)
        {
            return candidate;
        }
    }

    return ZERO;
}

void magics::generate()
{
    std::cout << "Rook Magic Numbers" << std::endl;
    for (int sq = 0; sq < 64; sq++)
    {
        printf("%d : 0x%llxULL\n", sq, generateMagicNumber(sq, tables::RELEVANT_BITS_COUNT_ROOK[sq], &tables::maskRookAttackRays, &tables::maskRookAttacks));
    }
    std::cout << std::endl;

    std::cout << "Bishop Magic Numbers" << std::endl;
    for (int sq = 0; sq < 64; sq++)
    {
        printf("%d : 0x%llxULL\n", sq, generateMagicNumber(sq, tables::RELEVANT_BITS_COUNT_BISHOP[sq], &tables::maskBishopAttackRays, &tables::maskBishopAttacks));
    }
    std::cout << std::endl;
}