#include "movegen.hpp"

#include "utils.hpp"
#include "attacks.hpp"
#include "tables.hpp"
#include "move.hpp"
#include "board.hpp"

void generateCastlingMoves(std::vector<Move> &moves_vec, const Board &board, const int &opponent, const int &castle_b_sq, const int &castle_c_sq, const int &castle_d_sq, const int &castle_e_sq, const int &castle_f_sq, const int &castle_g_sq, const int &castle_queen_mask, const int &castle_king_mask);
void generatePawnDoublePushes(std::vector<Move> &moves_vec, U64 &pawn_double_pushes, const int &pawn_double_push_offset);
void generatePawnSinglePushWithPromotion(std::vector<Move> &moves_vec, const U64 &pawn_single_pushes, const int &pawn_single_push_offset);
void generatePawnSinglePushNoPromotion(std::vector<Move> &moves_vec, const U64 &pawn_single_pushes, const int &pawn_single_push_offset);
void generatePawnCapturesWithPromotion(std::vector<Move> &moves_vec, const int &to_move, const U64 &to_move_pawns, const U64 &opponent_occupancies);
void generatePawnCapturesNoPromotion(std::vector<Move> &moves_vec, const int &to_move, const U64 &to_move_pawns, const U64 &opponent_occupancies);
void generateEnPassantCapture(std::vector<Move> &moves_vec, const U64 &to_move_pawns, const int &en_passant_square, const int &opponent);
void generateKnightMoves(std::vector<Move> &moves_vec, const U64 &to_move_knights, const U64 &to_move_occupancies, const U64 &opponent_occupancies);
void generateBishopMoves(std::vector<Move> &moves_vec, const U64 &to_move_bishops, const U64 &to_move_occupancies, const U64 &opponent_occupancies, const U64 &both_occupancies);
void generateRookMoves(std::vector<Move> &moves_vec, const U64 &to_move_rooks, const U64 &to_move_occupancies, const U64 &opponent_occupancies, const U64 &both_occupancies);
void generateQueenMoves(std::vector<Move> &moves_vec, const U64 &to_move_queens, const U64 &to_move_occupancies, const U64 &opponent_occupancies, const U64 &both_occupancies);
void generateKingMoves(std::vector<Move> &moves_vec, const U64 &to_move_king, const U64 &to_move_occupancies, const U64 &opponent_occupancies);

// TODO: improve correctness
// TODO: improve performance 10M NPS -> ~ 20 NPS
// TODO: do something about std::vector<Move>
std::vector<Move> Movegen::generatePseudoLegalMoves(const Board &board)
{
    std::vector<Move> moves_vec;

    U64 to_move = board.getSideToMove();
    int opponent = Utils::getOpponent(to_move);

    U64 to_move_pawns = board.getPieces(to_move, PAWN);

    U64 to_move_occupancies = board.getOccupancies(to_move);
    U64 opponent_occupancies = board.getOccupancies(opponent);
    U64 both_occupancies = board.getOccupancies(BOTH);

    // TODO: Improve this
    int castle_b_sq, castle_c_sq, castle_d_sq, castle_e_sq, castle_f_sq, castle_g_sq;
    int castle_king_mask, castle_queen_mask;
    int pawn_double_push_offset, pawn_single_push_offset;
    U64 pawn_double_pushes, pawn_single_pushes;
    if (to_move == WHITE)
    {
        castle_b_sq = B1;
        castle_c_sq = C1;
        castle_d_sq = D1;
        castle_e_sq = E1;
        castle_f_sq = F1;
        castle_g_sq = G1;
        castle_king_mask = CASTLE_KING_WHITE;
        castle_queen_mask = CASTLE_QUEEN_WHITE;

        pawn_double_push_offset = -16;
        pawn_single_push_offset = pawn_double_push_offset / 2;
        pawn_double_pushes = Attacks::maskWhitePawnDoublePushes(to_move_pawns, ~both_occupancies);
        pawn_single_pushes = Attacks::maskWhitePawnSinglePushes(to_move_pawns, ~both_occupancies);
    }
    else
    {
        castle_b_sq = Utils::mirrorRank(B1);
        castle_c_sq = Utils::mirrorRank(C1);
        castle_d_sq = Utils::mirrorRank(D1);
        castle_e_sq = Utils::mirrorRank(E1);
        castle_f_sq = Utils::mirrorRank(F1);
        castle_g_sq = Utils::mirrorRank(G1);
        castle_king_mask = CASTLE_KING_BLACK;
        castle_queen_mask = CASTLE_QUEEN_BLACK;

        pawn_double_push_offset = 16;
        pawn_single_push_offset = pawn_double_push_offset / 2;
        pawn_double_pushes = Attacks::maskBlackPawnDoublePushes(to_move_pawns, ~both_occupancies);
        pawn_single_pushes = Attacks::maskBlackPawnSinglePushes(to_move_pawns, ~both_occupancies);
    }

    // TODO: Move ordering
    generatePawnCapturesWithPromotion(moves_vec, to_move, to_move_pawns, opponent_occupancies);
    generatePawnCapturesNoPromotion(moves_vec, to_move, to_move_pawns, opponent_occupancies);
    generateEnPassantCapture(moves_vec, to_move_pawns, board.getEnPassantSquare(), opponent);
    generatePawnSinglePushWithPromotion(moves_vec, pawn_single_pushes, pawn_single_push_offset);
    generatePawnDoublePushes(moves_vec, pawn_double_pushes, pawn_double_push_offset);
    generatePawnSinglePushNoPromotion(moves_vec, pawn_single_pushes, pawn_single_push_offset);
    generateKnightMoves(moves_vec, board.getPieces(to_move, KNIGHT), to_move_occupancies, opponent_occupancies);
    generateBishopMoves(moves_vec, board.getPieces(to_move, BISHOP), to_move_occupancies, opponent_occupancies, both_occupancies);
    generateRookMoves(moves_vec, board.getPieces(to_move, ROOK), to_move_occupancies, opponent_occupancies, both_occupancies);
    generateQueenMoves(moves_vec, board.getPieces(to_move, QUEEN), to_move_occupancies, opponent_occupancies, both_occupancies);
    generateCastlingMoves(moves_vec, board, opponent, castle_b_sq, castle_c_sq, castle_d_sq, castle_e_sq, castle_f_sq, castle_g_sq, castle_queen_mask, castle_king_mask);
    generateKingMoves(moves_vec, board.getPieces(to_move, KING), to_move_occupancies, opponent_occupancies);

    return moves_vec;
}

std::vector<Move> Movegen::generateLegalMoves(const Board &board)
{
    std::vector<Move> legal_moves;
    for (Move move : Movegen::generatePseudoLegalMoves(board))
    {
        Board board_copy = board;
        if (board_copy.makeMove(move))
        {
            legal_moves.push_back(move);
        }
    }
    return legal_moves;
}

void generateCastlingMoves(std::vector<Move> &moves_vec, const Board &board, const int &opponent, const int &castle_b_sq, const int &castle_c_sq, const int &castle_d_sq, const int &castle_e_sq, const int &castle_f_sq, const int &castle_g_sq, const int &castle_queen_mask, const int &castle_king_mask)
{
    // Castling Moves
    if (!board.isSquareAttacked(castle_e_sq, opponent))
    {
        if ((board.getCastlingRights() & castle_king_mask) && !Utils::getBit(board.getOccupancies(BOTH), castle_f_sq) && !Utils::getBit(board.getOccupancies(BOTH), castle_g_sq))
        {
            // TODO: remove this last verification of attack
            // because we are already going to check if king is in check after the move
            if (!board.isSquareAttacked(castle_f_sq, opponent && !board.isSquareAttacked(castle_g_sq, opponent)))
            {
                moves_vec.push_back(Move(castle_e_sq, castle_g_sq, KING, 0, false, false, false, true));
            }
        }
        if ((board.getCastlingRights() & castle_queen_mask) && !Utils::getBit(board.getOccupancies(BOTH), castle_d_sq) && !Utils::getBit(board.getOccupancies(BOTH), castle_c_sq) && !Utils::getBit(board.getOccupancies(BOTH), castle_b_sq))
        {
            if (!board.isSquareAttacked(castle_d_sq, opponent) && !board.isSquareAttacked(castle_c_sq, opponent))
            {
                moves_vec.push_back(Move(castle_e_sq, castle_c_sq, KING, 0, false, false, false, true));
            }
        }
    }
}

void generatePawnDoublePushes(std::vector<Move> &moves_vec, U64 &pawn_double_pushes, const int &pawn_double_push_offset)
{
    // Pawn Double Pushes
    while (pawn_double_pushes)
    {
        int to_square = Utils::bitScan(pawn_double_pushes);
        int from_square = to_square + pawn_double_push_offset;
        moves_vec.push_back(Move(from_square, to_square, PAWN, 0, false, true, false, false));
        Utils::popLastBit(pawn_double_pushes);
    }
}

void generatePawnSinglePushWithPromotion(std::vector<Move> &moves_vec, const U64 &pawn_single_pushes, const int &pawn_single_push_offset)
{
    // Pawn Single Pushes With Promotion
    U64 pawn_single_pushes_promo = pawn_single_pushes & (Tables::MASK_RANK[0] | Tables::MASK_RANK[7]);
    while (pawn_single_pushes_promo)
    {
        int to_square = Utils::bitScan(pawn_single_pushes_promo);
        int from_square = to_square + pawn_single_push_offset;
        moves_vec.push_back(Move(from_square, to_square, PAWN, KNIGHT, false, false, false, false));
        moves_vec.push_back(Move(from_square, to_square, PAWN, BISHOP, false, false, false, false));
        moves_vec.push_back(Move(from_square, to_square, PAWN, ROOK, false, false, false, false));
        moves_vec.push_back(Move(from_square, to_square, PAWN, QUEEN, false, false, false, false));
        Utils::popLastBit(pawn_single_pushes_promo);
    }
}

void generatePawnSinglePushNoPromotion(std::vector<Move> &moves_vec, const U64 &pawn_single_pushes, const int &pawn_single_push_offset)
{
    // Pawn Single Pushes No Promotion
    U64 pawn_single_pushes_no_promo = pawn_single_pushes & Tables::MASK_CLEAR_RANK[0] & Tables::MASK_CLEAR_RANK[7];
    while (pawn_single_pushes_no_promo)
    {
        int to_square = Utils::bitScan(pawn_single_pushes_no_promo);
        int from_square = to_square + pawn_single_push_offset;
        moves_vec.push_back(Move(from_square, to_square, PAWN, 0, false, false, false, false));
        Utils::popLastBit(pawn_single_pushes_no_promo);
    }
}

void generatePawnCapturesWithPromotion(std::vector<Move> &moves_vec, const int &to_move, const U64 &to_move_pawns, const U64 &opponent_occupancies)
{
    // Pawn Captures With Promotion
    U64 pawns_can_capture_with_promo = to_move_pawns & Tables::MASK_RANK[6 - (5 * to_move)];
    while (pawns_can_capture_with_promo)
    {
        int from_square = Utils::bitScan(pawns_can_capture_with_promo);
        U64 pawn_captures_promo = Tables::ATTACKS_PAWN[to_move][from_square] & opponent_occupancies;
        while (pawn_captures_promo)
        {
            int to_square = Utils::bitScan(pawn_captures_promo);
            moves_vec.push_back(Move(from_square, to_square, PAWN, KNIGHT, true, false, false, false));
            moves_vec.push_back(Move(from_square, to_square, PAWN, BISHOP, true, false, false, false));
            moves_vec.push_back(Move(from_square, to_square, PAWN, ROOK, true, false, false, false));
            moves_vec.push_back(Move(from_square, to_square, PAWN, QUEEN, true, false, false, false));
            Utils::popLastBit(pawn_captures_promo);
        }
        Utils::popLastBit(pawns_can_capture_with_promo);
    }
}

void generatePawnCapturesNoPromotion(std::vector<Move> &moves_vec, const int &to_move, const U64 &to_move_pawns, const U64 &opponent_occupancies)
{
    // Pawns Captures No Promotion
    U64 pawns_can_capture_no_promo = to_move_pawns & Tables::MASK_CLEAR_RANK[6 - (5 * to_move)];
    while (pawns_can_capture_no_promo)
    {
        int from_square = Utils::bitScan(pawns_can_capture_no_promo);
        U64 pawn_captures_no_promo = Tables::ATTACKS_PAWN[to_move][from_square] & opponent_occupancies;
        while (pawn_captures_no_promo)
        {
            int to_square = Utils::bitScan(pawn_captures_no_promo);
            moves_vec.push_back(Move(from_square, to_square, PAWN, 0, true, false, false, false));
            Utils::popLastBit(pawn_captures_no_promo);
        }
        Utils::popLastBit(pawns_can_capture_no_promo);
    }
}

void generateEnPassantCapture(std::vector<Move> &moves_vec, const U64 &to_move_pawns, const int &en_passant_square, const int &opponent)
{
    // En-Passant Capture
    if (en_passant_square != -1)
    {
        U64 pawns_can_en_passant = Tables::ATTACKS_PAWN[opponent][en_passant_square] & to_move_pawns;
        while (pawns_can_en_passant)
        {
            int from_square = Utils::bitScan(pawns_can_en_passant);
            moves_vec.push_back(Move(from_square, en_passant_square, PAWN, 0, true, false, true, false));
            Utils::popLastBit(pawns_can_en_passant);
        }
    }
}

void generateKnightMoves(std::vector<Move> &moves_vec, const U64 &to_move_knights, const U64 &to_move_occupancies, const U64 &opponent_occupancies)
{
    // Knight Moves
    U64 knights = to_move_knights;
    while (knights)
    {
        int from_square = Utils::bitScan(knights);
        U64 moves = Tables::ATTACKS_KNIGHT[from_square] & ~to_move_occupancies;
        while (moves)
        {
            int to_square = Utils::bitScan(moves);
            moves_vec.push_back(Move(from_square, to_square, KNIGHT, 0, Utils::getBit(opponent_occupancies, to_square), false, false, false));
            Utils::popLastBit(moves);
        }
        Utils::popLastBit(knights);
    }
}

void generateBishopMoves(std::vector<Move> &moves_vec, const U64 &to_move_bishops, const U64 &to_move_occupancies, const U64 &opponent_occupancies, const U64 &both_occupancies)
{
    // Bishop Moves
    U64 bishops = to_move_bishops;
    while (bishops)
    {
        int from_square = Utils::bitScan(bishops);
        U64 moves = Tables::getBishopAttacks(from_square, both_occupancies) & ~to_move_occupancies;
        while (moves)
        {
            int to_square = Utils::bitScan(moves);
            moves_vec.push_back(Move(from_square, to_square, BISHOP, 0, Utils::getBit(opponent_occupancies, to_square), false, false, false));
            Utils::popLastBit(moves);
        }
        Utils::popLastBit(bishops);
    }
}

void generateRookMoves(std::vector<Move> &moves_vec, const U64 &to_move_rooks, const U64 &to_move_occupancies, const U64 &opponent_occupancies, const U64 &both_occupancies)
{
    // Rook Moves
    U64 rooks = to_move_rooks;
    while (rooks)
    {
        int from_square = Utils::bitScan(rooks);
        U64 moves = Tables::getRookAttacks(from_square, both_occupancies) & ~to_move_occupancies;
        while (moves)
        {
            int to_square = Utils::bitScan(moves);
            moves_vec.push_back(Move(from_square, to_square, ROOK, 0, Utils::getBit(opponent_occupancies, to_square), false, false, false));
            Utils::popLastBit(moves);
        }
        Utils::popLastBit(rooks);
    }
}

void generateQueenMoves(std::vector<Move> &moves_vec, const U64 &to_move_queens, const U64 &to_move_occupancies, const U64 &opponent_occupancies, const U64 &both_occupancies)
{
    // Queen Moves
    U64 queens = to_move_queens;
    while (queens)
    {
        int from_square = Utils::bitScan(queens);
        U64 moves = Tables::getQueenAttacks(from_square, both_occupancies) & ~to_move_occupancies;
        while (moves)
        {
            int to_square = Utils::bitScan(moves);
            moves_vec.push_back(Move(from_square, to_square, QUEEN, 0, Utils::getBit(opponent_occupancies, to_square), false, false, false));
            Utils::popLastBit(moves);
        }
        Utils::popLastBit(queens);
    }
}

void generateKingMoves(std::vector<Move> &moves_vec, const U64 &to_move_king, const U64 &to_move_occupancies, const U64 &opponent_occupancies)
{
    // King Moves
    U64 king = to_move_king;
    int from_square = Utils::bitScan(king);
    U64 moves = Tables::ATTACKS_KING[from_square] & ~to_move_occupancies;
    while (moves)
    {
        int to_square = Utils::bitScan(moves);
        moves_vec.push_back(Move(from_square, to_square, KING, 0, Utils::getBit(opponent_occupancies, to_square), false, false, false));
        Utils::popLastBit(moves);
    }
}