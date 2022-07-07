#include <engine/movegen/attacks.hpp>

#include <engine/movegen/utils.hpp>
#include <engine/movegen/bitboard.hpp>
#include <engine/movegen/tables.hpp>

namespace attacks
{
  U64 maskWhitePawnSinglePushes(U64 wpawns, U64 empty)
  {
    return bitboard::nortOne(wpawns) & empty;
  }

  U64 maskBlackPawnSinglePushes(U64 bpawns, U64 empty)
  {
    return bitboard::soutOne(bpawns) & empty;
  }

  U64 maskWhitePawnDoublePushes(U64 wpawns, U64 empty)
  {
    static const U64 rank4 = 0x00000000FF000000;
    U64 singlePushes = maskWhitePawnSinglePushes(wpawns, empty);
    return bitboard::nortOne(singlePushes) & empty & rank4;
  }

  U64 maskBlackPawnDoublePushes(U64 bpawns, U64 empty)
  {
    static const U64 rank5 = 0x000000FF00000000;
    U64 singlePushes = maskBlackPawnSinglePushes(bpawns, empty);
    return bitboard::soutOne(singlePushes) & empty & rank5;
  }

  inline U64 maskWhitePawnEastAttacks(U64 wpawns) { return bitboard::noEaOne(wpawns); }
  inline U64 maskWhitePawnWestAttacks(U64 wpawns) { return bitboard::noWeOne(wpawns); }
  inline U64 maskBlackPawnEastAttacks(U64 bpawns) { return bitboard::soEaOne(bpawns); }
  inline U64 maskBlackPawnWestAttacks(U64 bpawns) { return bitboard::soWeOne(bpawns); }

  U64 maskWhitePawnAnyAttacks(U64 wpawns)
  {
    return maskWhitePawnEastAttacks(wpawns) | maskWhitePawnWestAttacks(wpawns);
  }

  U64 maskBlackPawnAnyAttacks(U64 bpawns)
  {
    return maskBlackPawnEastAttacks(bpawns) | maskBlackPawnWestAttacks(bpawns);
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
    U64 attacks = bitboard::eastOne(kings) | bitboard::westOne(kings);
    kings |= attacks;
    attacks |= bitboard::nortOne(kings) | bitboard::soutOne(kings);
    return attacks;
  }

  U64 maskBishopAttackRays(int sq)
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

  U64 maskRookAttackRays(int sq)
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

  U64 maskBishopAttacks(int sq, U64 block)
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

  U64 maskRookAttacks(int sq, U64 block)
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

} // namespace attacks