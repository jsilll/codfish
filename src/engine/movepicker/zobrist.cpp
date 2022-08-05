#include <engine/movepicker/zobrist.hpp>

#include <random>

static u64 generate_random_number_u64()
{
    u64 n1 = ((u64)std::rand());
    u64 n2 = ((u64)std::rand());
    return n1 | (n2 << 32);
}

namespace zobrist
{
    u64 piece_keys[N_SIDES][N_PIECES][N_SQUARES];

    u64 en_passant_keys[N_SQUARES];

    u64 castle_keys[16];

    u64 side_key[BOTH] = {};

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

        side_key[1] = generate_random_number_u64();
    }

    u64 generate_hash_key(const Board &board)
    {
        u64 final_key = 0ULL;

        for (int piece = PAWN; piece < N_PIECES; piece++)
        {
            for (int side = WHITE; side < BOTH; side++)
            {
                u64 bitboard = board.get_pieces(side, piece);

                while (bitboard)
                {
                    int sq = bitboard::bit_scan_forward(bitboard);

                    final_key ^= piece_keys[side][piece][sq];

                    bitboard::pop_bit(bitboard, sq);
                }
            }
        }

        if (board.get_en_passant_square() != EMPTY_SQUARE)
        {
            final_key ^= en_passant_keys[board.get_en_passant_square()];
        }

        final_key ^= castle_keys[board.get_castling_rights()];

        // if (board.get_side_to_move() == BLACK)
        // {
        //     final_key ^= side_key[1];
        // }
        // final_key ^= board.get_side_to_move() == WHITE ? 0ULL : side_key;
        final_key ^= side_key[board.get_side_to_move()];

        return final_key;
    }

} // namespace zobrist
