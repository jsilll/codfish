#include "utils.hpp"
#include "board.hpp"
#include "eval.hpp"

// clang-format off
int MG_PAWN_TABLE[64] = {
      0,   0,   0,   0,   0,   0,  0,   0,
     98, 134,  61,  95,  68, 126, 34, -11,
     -6,   7,  26,  31,  65,  56, 25, -20,
    -14,  13,   6,  21,  23,  12, 17, -23,
    -27,  -2,  -5,  12,  17,   6, 10, -25,
    -26,  -4,  -4, -10,   3,   3, 33, -12,
    -35,  -1, -20, -23, -15,  24, 38, -22,
      0,   0,   0,   0,   0,   0,  0,   0,
};

int MG_KNIGHT_TABLE[64] = {
    -167, -89, -34, -49,  61, -97, -15, -107,
     -73, -41,  72,  36,  23,  62,   7,  -17,
     -47,  60,  37,  65,  84, 129,  73,   44,
      -9,  17,  19,  53,  37,  69,  18,   22,
     -13,   4,  16,  13,  28,  19,  21,   -8,
     -23,  -9,  12,  10,  19,  17,  25,  -16,
     -29, -53, -12,  -3,  -1,  18, -14,  -19,
    -105, -21, -58, -33, -17, -28, -19,  -23,
};

int MG_BISHOP_TABLE[64] = {
    -29,   4, -82, -37, -25, -42,   7,  -8,
    -26,  16, -18, -13,  30,  59,  18, -47,
    -16,  37,  43,  40,  35,  50,  37,  -2,
     -4,   5,  19,  50,  37,  37,   7,  -2,
     -6,  13,  13,  26,  34,  12,  10,   4,
      0,  15,  15,  15,  14,  27,  18,  10,
      4,  15,  16,   0,   7,  21,  33,   1,
    -33,  -3, -14, -21, -13, -12, -39, -21,
};

int MG_ROOK_TABLE[64] = {
     32,  42,  32,  51, 63,  9,  31,  43,
     27,  32,  58,  62, 80, 67,  26,  44,
     -5,  19,  26,  36, 17, 45,  61,  16,
    -24, -11,   7,  26, 24, 35,  -8, -20,
    -36, -26, -12,  -1,  9, -7,   6, -23,
    -45, -25, -16, -17,  3,  0,  -5, -33,
    -44, -16, -20,  -9, -1, 11,  -6, -71,
    -19, -13,   1,  17, 16,  7, -37, -26,
};

int MG_QUEEN_TABLE[64] = {
    -28,   0,  29,  12,  59,  44,  43,  45,
    -24, -39,  -5,   1, -16,  57,  28,  54,
    -13, -17,   7,   8,  29,  56,  47,  57,
    -27, -27, -16, -16,  -1,  17,  -2,   1,
     -9, -26,  -9, -10,  -2,  -4,   3,  -3,
    -14,   2, -11,  -2,  -5,   2,  14,   5,
    -35,  -8,  11,   2,   8,  15,  -3,   1,
     -1, -18,  -9,  10, -15, -25, -31, -50,
};

int MG_KING_TABLE[64] = {
    -65,  23,  16, -15, -56, -34,   2,  13,
     29,  -1, -20,  -7,  -8,  -4, -38, -29,
     -9,  24,   2, -16, -20,   6,  22, -22,
    -17, -20, -12, -27, -30, -25, -14, -36,
    -49,  -1, -27, -39, -46, -44, -33, -51,
    -14, -14, -22, -46, -44, -30, -15, -27,
      1,   7,  -8, -64, -43, -16,   9,   8,
    -15,  36,  12, -54,   8, -28,  24,  14,
};

int EG_PAWN_TABLE[64] = {
      0,   0,   0,   0,   0,   0,   0,   0,
    178, 173, 158, 134, 147, 132, 165, 187,
     94, 100,  85,  67,  56,  53,  82,  84,
     32,  24,  13,   5,  -2,   4,  17,  17,
     13,   9,  -3,  -7,  -7,  -8,   3,  -1,
      4,   7,  -6,   1,   0,  -5,  -1,  -8,
     13,   8,   8,  10,  13,   0,   2,  -7,
      0,   0,   0,   0,   0,   0,   0,   0,
};

int EG_KNIGHT_TABLE[64] = {
    -58, -38, -13, -28, -31, -27, -63, -99,
    -25,  -8, -25,  -2,  -9, -25, -24, -52,
    -24, -20,  10,   9,  -1,  -9, -19, -41,
    -17,   3,  22,  22,  22,  11,   8, -18,
    -18,  -6,  16,  25,  16,  17,   4, -18,
    -23,  -3,  -1,  15,  10,  -3, -20, -22,
    -42, -20, -10,  -5,  -2, -20, -23, -44,
    -29, -51, -23, -15, -22, -18, -50, -64,
};

int EG_BISHOP_TABLE[64] = {
    -14, -21, -11,  -8, -7,  -9, -17, -24,
     -8,  -4,   7, -12, -3, -13,  -4, -14,
      2,  -8,   0,  -1, -2,   6,   0,   4,
     -3,   9,  12,   9, 14,  10,   3,   2,
     -6,   3,  13,  19,  7,  10,  -3,  -9,
    -12,  -3,   8,  10, 13,   3,  -7, -15,
    -14, -18,  -7,  -1,  4,  -9, -15, -27,
    -23,  -9, -23,  -5, -9, -16,  -5, -17,
};

int EG_ROOK_TABLE[64] = {
    13, 10, 18, 15, 12,  12,   8,   5,
    11, 13, 13, 11, -3,   3,   8,   3,
     7,  7,  7,  5,  4,  -3,  -5,  -3,
     4,  3, 13,  1,  2,   1,  -1,   2,
     3,  5,  8,  4, -5,  -6,  -8, -11,
    -4,  0, -5, -1, -7, -12,  -8, -16,
    -6, -6,  0,  2, -9,  -9, -11,  -3,
    -9,  2,  3, -1, -5, -13,   4, -20,
};

int EG_QUEEN_TABLE[64] = {
     -9,  22,  22,  27,  27,  19,  10,  20,
    -17,  20,  32,  41,  58,  25,  30,   0,
    -20,   6,   9,  49,  47,  35,  19,   9,
      3,  22,  24,  45,  57,  40,  57,  36,
    -18,  28,  19,  47,  31,  34,  39,  23,
    -16, -27,  15,   6,   9,  17,  10,   5,
    -22, -23, -30, -16, -16, -23, -36, -32,
    -33, -28, -22, -43,  -5, -32, -20, -41,
};

int EG_KING_TABLE[64] = {
    -74, -35, -18, -18, -11,  15,   4, -17,
    -12,  17,  14,  17,  17,  38,  23,  11,
     10,  17,  23,  15,  20,  45,  44,  13,
     -8,  22,  24,  27,  26,  33,  26,   3,
    -18,  -4,  21,  24,  27,  23,   9, -11,
    -19,  -3,  11,  21,  23,  16,   7,  -9,
    -27, -11,   4,  13,  14,   4,  -5, -17,
    -53, -34, -21, -11, -28, -14, -24, -43
};
// clang-format on

int MG_TABLE[2][6][64];
int EG_TABLE[2][6][64];
int GAME_PHASE_INC[6] = {0, 1, 1, 2, 4, 0};

void Eval::init()
{
    for (int sq = A1; sq < N_SQUARES; sq++)
    {
        int sq_flipped = Utils::flipSquare(sq);

        MG_TABLE[WHITE][PAWN][sq] = MG_PAWN_TABLE[sq_flipped];
        MG_TABLE[WHITE][KNIGHT][sq] = MG_KNIGHT_TABLE[sq_flipped];
        MG_TABLE[WHITE][BISHOP][sq] = MG_BISHOP_TABLE[sq_flipped];
        MG_TABLE[WHITE][ROOK][sq] = MG_ROOK_TABLE[sq_flipped];
        MG_TABLE[WHITE][QUEEN][sq] = MG_QUEEN_TABLE[sq_flipped];
        MG_TABLE[WHITE][KING][sq] = MG_KING_TABLE[sq_flipped];

        MG_TABLE[BLACK][PAWN][sq] = MG_PAWN_TABLE[sq];
        MG_TABLE[BLACK][KNIGHT][sq] = MG_KNIGHT_TABLE[sq];
        MG_TABLE[BLACK][BISHOP][sq] = MG_BISHOP_TABLE[sq];
        MG_TABLE[BLACK][ROOK][sq] = MG_ROOK_TABLE[sq];
        MG_TABLE[BLACK][QUEEN][sq] = MG_QUEEN_TABLE[sq];
        MG_TABLE[BLACK][KING][sq] = MG_KING_TABLE[sq];

        EG_TABLE[WHITE][PAWN][sq] = EG_PAWN_TABLE[sq_flipped];
        EG_TABLE[WHITE][KNIGHT][sq] = EG_KNIGHT_TABLE[sq_flipped];
        EG_TABLE[WHITE][BISHOP][sq] = EG_BISHOP_TABLE[sq_flipped];
        EG_TABLE[WHITE][ROOK][sq] = EG_ROOK_TABLE[sq_flipped];
        EG_TABLE[WHITE][QUEEN][sq] = EG_QUEEN_TABLE[sq_flipped];
        EG_TABLE[WHITE][KING][sq] = EG_KING_TABLE[sq_flipped];

        EG_TABLE[BLACK][PAWN][sq] = EG_PAWN_TABLE[sq];
        EG_TABLE[BLACK][KNIGHT][sq] = EG_KNIGHT_TABLE[sq];
        EG_TABLE[BLACK][BISHOP][sq] = EG_BISHOP_TABLE[sq];
        EG_TABLE[BLACK][ROOK][sq] = EG_ROOK_TABLE[sq];
        EG_TABLE[BLACK][QUEEN][sq] = EG_QUEEN_TABLE[sq];
        EG_TABLE[BLACK][KING][sq] = EG_KING_TABLE[sq];
    }
}

int Eval::eval(Board &board)
{
    int mg[2]{};
    int eg[2]{};
    int game_phase = 0;

    for (int piece_type = PAWN; piece_type < EMPTY; piece_type++)
    {
        U64 pieces_white = board.getPieces(WHITE, piece_type);
        while (pieces_white)
        {
            int sq = Utils::bitScanForward(pieces_white);
            mg[WHITE] += MG_TABLE[WHITE][piece_type][sq];
            eg[WHITE] += EG_TABLE[WHITE][piece_type][sq];
            game_phase += GAME_PHASE_INC[piece_type];
            Utils::popBit(pieces_white, sq);
        }

        U64 pieces_black = board.getPieces(BLACK, piece_type);
        while (pieces_black)
        {
            int sq = Utils::bitScanForward(pieces_black);
            mg[BLACK] += MG_TABLE[BLACK][piece_type][sq];
            eg[BLACK] += EG_TABLE[BLACK][piece_type][sq];
            game_phase += GAME_PHASE_INC[piece_type];
            Utils::popBit(pieces_black, sq);
        }
    }

    int to_move = board.getSideToMove();
    int opponent = Utils::getOpponent(to_move); // TODO: add this method to board

    int mg_score = mg[to_move] - mg[opponent];
    int eg_score = eg[to_move] - eg[opponent];
    int mg_phase = game_phase < 24 ? game_phase : 24;
    int eg_phase = 24 - mg_phase;

    return (mg_score * mg_phase + eg_score * eg_phase) / 24;
}