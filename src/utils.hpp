#pragma once

#include "defs.hpp"

#include <string>
#include <vector>

namespace Utils
{

  inline int getSquare(const int rk, const int fl)
  {
    return 8 * rk + fl;
  }
  inline int getFile(const int sq) { return sq & 7; }
  inline int getRank(const int sq) { return sq >> 3; }

  inline int getOpponent(int to_move) { return to_move ^ 1; }

  inline void setBit(U64 &bb, int sq) { bb |= (1ULL << sq); }
  inline void popBit(U64 &bn, int sq) { bn &= ~(1ULL << sq); }
  inline void popLastBit(U64 &bb) { bb &= bb - 1; }
  inline bool getBit(U64 bb, int sq) { return ((bb >> sq) & 1ULL); }

  inline unsigned int bitCount(U64 bb)
  {
    // Brian Kernighan's way
    unsigned int count = 0;
    while (bb)
    {
      count++;
      bb &= bb - 1;
    }
    return count;
  }

  int bitScan(U64 bb);
  int bitScanForward(U64 bb);

  inline U64 soutOne(U64 bb) { return (bb >> 8); }
  inline U64 nortOne(U64 bb) { return (bb << 8); }
  inline U64 eastOne(U64 bb) { return (bb << 1) & 0xFEFEFEFEFEFEFEFE; }
  inline U64 noEaOne(U64 bb) { return (bb << 9) & 0xFEFEFEFEFEFEFEFE; }
  inline U64 soEaOne(U64 bb) { return (bb >> 7) & 0xFEFEFEFEFEFEFEFE; }
  inline U64 westOne(U64 bb) { return (bb >> 1) & 0x7F7F7F7F7F7F7F7F; }
  inline U64 soWeOne(U64 bb) { return (bb >> 9) & 0x7F7F7F7F7F7F7F7F; }
  inline U64 noWeOne(U64 bb) { return (bb << 7) & 0x7F7F7F7F7F7F7F7F; }

  U64 setOccupancy(int index, int bits_in_mask, U64 attack_mask);

  void printBB(U64 bb);

  /**
   * @brief Splits a string by spaces
   *
   * @param text
   * @return std::vector<std::string>
   */
  std::vector<std::string> tokenizeString(const std::string &text);

} // namespace Utils