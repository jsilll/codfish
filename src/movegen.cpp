#include "movegen.hpp"
#include "utils.hpp"
#include "tables.hpp"
#include "magics.hpp"
#include "board.hpp"

inline U64 getBishopAttacks(const int &sq, U64 occ)
{
    occ &= Magics::MAGIC_TABLE_BISHOP[sq].mask;
    occ *= Magics::MAGIC_TABLE_BISHOP[sq].magic;
    occ >>= Magics::MAGIC_TABLE_BISHOP[sq].shift;
    return Tables::ATTACKS_BISHOP[sq][occ];
}

inline U64 getRookAttacks(const int &sq, U64 occ)
{
    occ &= Magics::MAGIC_TABLE_ROOK[sq].mask;
    occ *= Magics::MAGIC_TABLE_ROOK[sq].magic;
    occ >>= Magics::MAGIC_TABLE_ROOK[sq].shift;
    return Tables::ATTACKS_ROOK[sq][occ];
}

inline U64 getQueenAttacks(const int &sq, U64 occ)
{
    return getBishopAttacks(sq, occ) | getRookAttacks(sq, occ);
}

bool Board::isSquareAttacked(const int sq, const Color side_to_attack) const
{
    switch (side_to_attack) // TODO: maybe split in two separate functions
    {
    case WHITE:
        if (getRookAttacks(sq, _occupied_squares) & (_white_rooks | _white_queens))
        {
            return true;
        }
        else if (getBishopAttacks(sq, _occupied_squares) & (_white_bishops | _white_queens))
        {
            return true;
        }
        else if (Tables::ATTACKS_KNIGHT[sq] & _white_knights)
        {
            return true;
        }
        else if (Tables::ATTACKS_PAWN[BLACK][sq] & _white_pawns)
        {
            return true;
        }
        else if (Tables::ATTACKS_KING[sq] & _white_king)
        {
            return true;
        }
        break;
    case BLACK:
        if (getRookAttacks(sq, _occupied_squares) & (_black_rooks | _black_queens))
        {
            return true;
        }
        else if (getBishopAttacks(sq, _occupied_squares) & (_black_bishops | _black_queens))
        {
            return true;
        }
        else if (Tables::ATTACKS_KNIGHT[sq] & _black_knights)
        {
            return true;
        }
        else if (Tables::ATTACKS_PAWN[WHITE][sq] & _black_pawns)
        {
            return true;
        }
        else if (Tables::ATTACKS_KING[sq] & _black_king)
        {
            return true;
        }
        break;
    default:
        break;
    }
    return false;
}

void Movegen::generateMoves(Board &board)
{
}