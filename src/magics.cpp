#include "magics.hpp"

#include "utils.hpp"
#include "tables.hpp"
#include <iostream>
#include <cstring>

// clang-format off
const int RELEVANT_BITS_COUNT_BISHOP[64] = {
    6,5,5,5,5,5,5,6,
    5,5,5,5,5,5,5,5,
    5,5,7,7,7,7,5,5,
    5,5,7,9,9,7,5,5,
    5,5,7,9,9,7,5,5,
    5,5,7,7,7,7,5,5,
    5,5,5,5,5,5,5,5,
    6,5,5,5,5,5,5,6,
};

const int RELEVANT_BITS_COUNT_ROOK[64] = {
    12,11,11,11,11,11,11,12,
    11,10,10,10,10,10,10,11,
    11,10,10,10,10,10,10,11,
    11,10,10,10,10,10,10,11,
    11,10,10,10,10,10,10,11,
    11,10,10,10,10,10,10,11,
    11,10,10,10,10,10,10,11,
    12,11,11,11,11,11,11,12,
};
// clang-format on

U64 maskBishopAttacks(int sq)
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

U64 maskRookAttacks(int sq)
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

U64 maskBishopAttacks(int sq, U64 block)
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

U64 maskRookAttacks(int sq, U64 block)
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

U64 setOccupancy(int sq, int bits_in_mask, U64 attack_mask)
{
    U64 occupancy = ZERO;
    for (int i = 0; i < bits_in_mask; i++)
    {
        int lsb_sq = utils::bitScan(attack_mask);
        utils::popBit(attack_mask, lsb_sq);
        if (sq & (1 << i))
        {
            occupancy |= tables::SQUARE_BB[lsb_sq];
        }
    }
    return occupancy;
}

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
        occupancies[i] = setOccupancy(i, relevant_bits, attack_mask);
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

void magics::init()
{
    std::cout << "ROOK MAGICS" << std::endl;
    for (int sq = 0; sq < 64; sq++)
    {
        printf(" 0x%llxULL\n", generateMagicNumber(sq, RELEVANT_BITS_COUNT_ROOK[sq], &maskRookAttacks, &maskRookAttacks));
    }

    std::cout << "BISHOP MAGICS" << std::endl;
    for (int sq = 0; sq < 64; sq++)
    {
        printf(" 0x%llxULL\n", generateMagicNumber(sq, RELEVANT_BITS_COUNT_BISHOP[sq], &maskBishopAttacks, &maskBishopAttacks));
    }
}