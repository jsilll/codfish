#include "utils.hpp"

#include "tables.hpp"
#include <iostream>

int Utils::bitScan(U64 bb) {
  if (bb) {
    return Utils::bitCount((bb & -bb) - 1);
  } else {
    return -1;
  }
}

U64 Utils::setOccupancy(int index, int bits_in_mask, U64 attack_mask) {
  U64 occupancy = ZERO;
  for (int bit = 0; bit < bits_in_mask; bit++) {
    int lsb_sq = Utils::bitScan(attack_mask);
    Utils::popBit(attack_mask, lsb_sq);
    if (index & (1 << bit)) {
      occupancy |= Tables::SQUARE_BB[lsb_sq];
    }
  }
  return occupancy;
}

void Utils::printBB(U64 bb) {
  for (int i = 7; i >= 0; i--) {
    std::cout << i + 1 << "  ";
    for (int n = 0; n < 8; n++) {
      std::cout << ((bb >> getSquare(i, n)) & ONE) << " ";
    }
    std::cout << "\n";
  }
  std::cout << "\n   a b c d e f g h\n\n";
  printf("bitboard %llud\n", bb);
}
