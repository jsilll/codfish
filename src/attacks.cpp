#include "attacks.hpp"

#include "utils.hpp"
#include "tables.hpp"

namespace attacks
{

  U64 maskWhitePawnSinglePushes(U64 wpawns, U64 empty)
  {
    return utils::nortOne(wpawns) & empty;
  }

  U64 maskBlackPawnSinglePushes(U64 bpawns, U64 empty)
  {
    return utils::soutOne(bpawns) & empty;
  }

  U64 maskWhitePawnDoublePushes(U64 wpawns, U64 empty)
  {
    static const U64 rank4 = 0x00000000FF000000;
    U64 singlePushs = maskWhitePawnSinglePushes(wpawns, empty);
    return utils::nortOne(singlePushs) & empty & rank4;
  }

  U64 maskBlackPawnDoublePushes(U64 bpawns, U64 empty)
  {
    static const U64 rank5 = 0x000000FF00000000;
    U64 singlePushs = maskBlackPawnSinglePushes(bpawns, empty);
    return utils::soutOne(singlePushs) & empty & rank5;
  }

  inline U64 maskWhitePawnEastAttacks(U64 wpawns) { return utils::noEaOne(wpawns); }
  inline U64 maskWhitePawnWestAttacks(U64 wpawns) { return utils::noWeOne(wpawns); }
  inline U64 maskBlackPawnEastAttacks(U64 bpawns) { return utils::soEaOne(bpawns); }
  inline U64 maskBlackPawnWestAttacks(U64 bpawns) { return utils::soWeOne(bpawns); }

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
    U64 attacks = utils::eastOne(kings) | utils::westOne(kings);
    kings |= attacks;
    attacks |= utils::nortOne(kings) | utils::soutOne(kings);
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
}