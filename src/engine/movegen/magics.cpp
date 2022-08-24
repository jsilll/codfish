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
  static const u64 MAGICS_BISHOP[N_SQUARES] = {
      0x40040844404084ULL,
      0x2004208a004208ULL,
      0x10190041080202ULL,
      0x108060845042010ULL,
      0x581104180800210ULL,
      0x2112080446200010ULL,
      0x1080820820060210ULL,
      0x3c0808410220200ULL,
      0x4050404440404ULL,
      0x21001420088ULL,
      0x24d0080801082102ULL,
      0x1020a0a020400ULL,
      0x40308200402ULL,
      0x4011002100800ULL,
      0x401484104104005ULL,
      0x801010402020200ULL,
      0x400210c3880100ULL,
      0x404022024108200ULL,
      0x810018200204102ULL,
      0x4002801a02003ULL,
      0x85040820080400ULL,
      0x810102c808880400ULL,
      0xe900410884800ULL,
      0x8002020480840102ULL,
      0x220200865090201ULL,
      0x2010100a02021202ULL,
      0x152048408022401ULL,
      0x20080002081110ULL,
      0x4001001021004000ULL,
      0x800040400a011002ULL,
      0xe4004081011002ULL,
      0x1c004001012080ULL,
      0x8004200962a00220ULL,
      0x8422100208500202ULL,
      0x2000402200300c08ULL,
      0x8646020080080080ULL,
      0x80020a0200100808ULL,
      0x2010004880111000ULL,
      0x623000a080011400ULL,
      0x42008c0340209202ULL,
      0x209188240001000ULL,
      0x400408a884001800ULL,
      0x110400a6080400ULL,
      0x1840060a44020800ULL,
      0x90080104000041ULL,
      0x201011000808101ULL,
      0x1a2208080504f080ULL,
      0x8012020600211212ULL,
      0x500861011240000ULL,
      0x180806108200800ULL,
      0x4000020e01040044ULL,
      0x300000261044000aULL,
      0x802241102020002ULL,
      0x20906061210001ULL,
      0x5a84841004010310ULL,
      0x4010801011c04ULL,
      0xa010109502200ULL,
      0x4a02012000ULL,
      0x500201010098b028ULL,
      0x8040002811040900ULL,
      0x28000010020204ULL,
      0x6000020202d0240ULL,
      0x8918844842082200ULL,
      0x4010011029020020ULL};

  static const u64 MAGICS_ROOK[N_SQUARES] = {
      0x8a80104000800020ULL,
      0x140002000100040ULL,
      0x2801880a0017001ULL,
      0x100081001000420ULL,
      0x200020010080420ULL,
      0x3001c0002010008ULL,
      0x8480008002000100ULL,
      0x2080088004402900ULL,
      0x800098204000ULL,
      0x2024401000200040ULL,
      0x100802000801000ULL,
      0x120800800801000ULL,
      0x208808088000400ULL,
      0x2802200800400ULL,
      0x2200800100020080ULL,
      0x801000060821100ULL,
      0x80044006422000ULL,
      0x100808020004000ULL,
      0x12108a0010204200ULL,
      0x140848010000802ULL,
      0x481828014002800ULL,
      0x8094004002004100ULL,
      0x4010040010010802ULL,
      0x20008806104ULL,
      0x100400080208000ULL,
      0x2040002120081000ULL,
      0x21200680100081ULL,
      0x20100080080080ULL,
      0x2000a00200410ULL,
      0x20080800400ULL,
      0x80088400100102ULL,
      0x80004600042881ULL,
      0x4040008040800020ULL,
      0x440003000200801ULL,
      0x4200011004500ULL,
      0x188020010100100ULL,
      0x14800401802800ULL,
      0x2080040080800200ULL,
      0x124080204001001ULL,
      0x200046502000484ULL,
      0x480400080088020ULL,
      0x1000422010034000ULL,
      0x30200100110040ULL,
      0x100021010009ULL,
      0x2002080100110004ULL,
      0x202008004008002ULL,
      0x20020004010100ULL,
      0x2048440040820001ULL,
      0x101002200408200ULL,
      0x40802000401080ULL,
      0x4008142004410100ULL,
      0x2060820c0120200ULL,
      0x1001004080100ULL,
      0x20c020080040080ULL,
      0x2935610830022400ULL,
      0x44440041009200ULL,
      0x280001040802101ULL,
      0x2100190040002085ULL,
      0x80c0084100102001ULL,
      0x4024081001000421ULL,
      0x20030a0244872ULL,
      0x12001008414402ULL,
      0x2006104900a0804ULL,
      0x1004081002402ULL};

  static Magic MAGIC_TABLE_BISHOP[N_SQUARES];
  static Magic MAGIC_TABLE_ROOK[N_SQUARES];

  static inline u64 generate_dense_random_number_u64()
  {
    u64 n1 = ((u64)std::rand()) & 0xFFFF;
    u64 n2 = ((u64)std::rand()) & 0xFFFF;
    u64 n3 = ((u64)std::rand()) & 0xFFFF;
    u64 n4 = ((u64)std::rand()) & 0xFFFF;
    return n1 | (n2 << 16) | (n3 << 32) | (n4 << 48);
  }

  static inline u64 get_magic_number_candidate()
  {
    return generate_dense_random_number_u64() & generate_dense_random_number_u64() & generate_dense_random_number_u64();
  }

  static u64 generate_magic_number(Square sq, int relevant_bits, u64 (*mask_attacks_fun)(Square), u64 (*mask_attacks_occ_fun)(Square, u64))
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
    for (int sq = A1; sq < N_SQUARES; sq++)
    {
      int bit_count = tables::get_relevant_bits_count_rook((Square)sq);
      u64 magic = generate_magic_number((Square)sq, bit_count, &attacks::mask_rook_attack_rays, &attacks::mask_rook_xray_attacks);
      printf("%d : 0x%lxULL\n", sq, magic);
    }
    std::cout << std::endl;

    std::cout << "Bishop Magic Numbers" << std::endl;
    for (int sq = A1; sq < N_SQUARES; sq++)
    {
      int bit_count = tables::get_relevant_bits_count_bishop((Square)sq);
      u64 magic = generate_magic_number((Square)sq, bit_count, &attacks::mask_bishop_attack_rays, &attacks::mask_bishop_xray_attacks);
      printf("%d : 0x%lxULL\n", sq, magic);
    }
    std::cout << std::endl;
  }

  void init()
  {
    for (int sq = A1; sq < N_SQUARES; sq++)
    {
      MAGIC_TABLE_BISHOP[sq].mask = attacks::mask_bishop_attack_rays((Square)sq);
      MAGIC_TABLE_BISHOP[sq].magic = MAGICS_BISHOP[sq];
      MAGIC_TABLE_BISHOP[sq].shift = 64 - tables::get_relevant_bits_count_bishop((Square)sq);
    }

    for (int sq = A1; sq < N_SQUARES; sq++)
    {
      MAGIC_TABLE_ROOK[sq].mask = attacks::mask_rook_attack_rays((Square)sq);
      MAGIC_TABLE_ROOK[sq].magic = MAGICS_ROOK[sq];
      MAGIC_TABLE_ROOK[sq].shift = 64 - tables::get_relevant_bits_count_rook((Square)sq);
    }
  }

  Magic get_bishop_magic(Square sq)
  {
    return MAGIC_TABLE_BISHOP[sq];
  }

  Magic get_rook_magic(Square sq)
  {
    return MAGIC_TABLE_ROOK[sq];
  }

} // namespace magics