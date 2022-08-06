#include <engine/movegen/magics.hpp>

#include <engine/utils.hpp>
#include <engine/bitboard.hpp>
#include <engine/movegen/tables.hpp>
#include <engine/movegen/attacks.hpp>

#include <iostream>
#include <cstring>
#include <random>

namespace magics
{
  Magic MAGIC_TABLE_BISHOP[N_SQUARES];
  Magic MAGIC_TABLE_ROOK[N_SQUARES];

  static u64 generate_dense_random_number_u64()
  {
    u64 n1 = ((u64)std::rand()) & 0xFFFF;
    u64 n2 = ((u64)std::rand()) & 0xFFFF;
    u64 n3 = ((u64)std::rand()) & 0xFFFF;
    u64 n4 = ((u64)std::rand()) & 0xFFFF;
    return n1 | (n2 << 16) | (n3 << 32) | (n4 << 48);
  }

  static inline u64 get_magic_number_candidate() { return generate_dense_random_number_u64() & generate_dense_random_number_u64() & generate_dense_random_number_u64(); }

  static u64 generate_magic_number(int sq, int relevant_bits, u64 (*mask_attacks_fun)(int), u64 (*mask_attacks_occ_fun)(int, u64))
  {
    int occupancy_indices = 1 << relevant_bits;
    u64 attack_mask = mask_attacks_fun(sq);
    u64 occupancies[4096], attacks[4096], used_attacks[4096];

    for (int i = 0; i < occupancy_indices; i++)
    {
      occupancies[i] = bitboard::set_occupancy(i, relevant_bits, attack_mask);
      attacks[i] = mask_attacks_occ_fun(sq, occupancies[i]);
    }

    for (int max_tries = 0; max_tries < 99999999; max_tries++)
    {
      u64 candidate = get_magic_number_candidate();

      if (bitboard::bit_count((attack_mask * candidate) & 0xFF00000000000000) < 6)
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
        else
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

  void generate()
  {
    std::cout << "Rook Magic Numbers" << std::endl;
    for (int sq = 0; sq < 64; sq++)
    {
      int bit_count = tables::RELEVANT_BITS_COUNT_ROOK[sq];
      u64 magic = generate_magic_number(sq, bit_count, &attacks::mask_rook_attack_rays, &attacks::mask_rook_attacks);
      printf("%d : 0x%lxULL\n", sq, magic);
    }
    std::cout << std::endl;

    std::cout << "Bishop Magic Numbers" << std::endl;
    for (int sq = 0; sq < 64; sq++)
    {
      int bit_count = tables::RELEVANT_BITS_COUNT_BISHOP[sq];
      u64 magic = generate_magic_number(sq, bit_count, &attacks::mask_bishop_attack_rays, &attacks::mask_bishop_attacks);
      printf("%d : 0x%lxULL\n", sq, magic);
    }
    std::cout << std::endl;
  }

  void init()
  {
    for (int sq = A1; sq < N_SQUARES; sq++)
    {
      MAGIC_TABLE_BISHOP[sq].mask = attacks::mask_bishop_attack_rays(sq);
      MAGIC_TABLE_BISHOP[sq].magic = MAGICS_BISHOP[sq];
      MAGIC_TABLE_BISHOP[sq].shift = 64 - tables::RELEVANT_BITS_COUNT_BISHOP[sq];
    }

    for (int sq = A1; sq < N_SQUARES; sq++)
    {
      MAGIC_TABLE_ROOK[sq].mask = attacks::mask_rook_attack_rays(sq);
      MAGIC_TABLE_ROOK[sq].magic = MAGICS_ROOK[sq];
      MAGIC_TABLE_ROOK[sq].shift = 64 - tables::RELEVANT_BITS_COUNT_ROOK[sq];
    }
  }

} // namespace magics