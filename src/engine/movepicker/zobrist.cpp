#include <engine/movepicker/zobrist.hpp>

#include <random>

static u64 piece_keys[N_SIDES][N_PIECES][N_SQUARES];

static u64 en_passant_keys[N_SQUARES];

static u64 castle_keys[16];

static u64 side_key;

static u64 generate_random_number_u64()
{
    u64 n1 = ((u64)std::rand());
    u64 n2 = ((u64)std::rand());
    return n1 | (n2 << 32);
}

namespace zobrist
{
    void init()
    {

        for (int piece = PAWN; piece < N_PIECES; piece++)
        {
            for (int square = A1; square < N_SQUARES; square++)
            {
                piece_keys[WHITE][piece][square] = generate_random_number_u64();
                piece_keys[BLACK][piece][square] = generate_random_number_u64();
            }
        }

        for (int square = A1; square < N_SQUARES; square++)
        {
            en_passant_keys[square] = generate_random_number_u64();
        }

        for (int castle_state = 0; castle_state < 16; castle_state++)
        {
            castle_keys[castle_state] = generate_random_number_u64();
        }

        side_key = generate_random_number_u64();
    }

} // namespace zobrist
