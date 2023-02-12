#include <codlib/zobrist.hpp>

namespace zobrist
{
    u64 side_key[BOTH]{};

    u64 en_passant_keys[N_SQUARES];

    u64 castle_keys[N_CASTLING_KEYS];

    u64 piece_keys[N_SIDES][N_PIECES][N_SQUARES];

} // namespace zobrist
