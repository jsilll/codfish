#define CATCH_CONFIG_MAIN

#include "../catch.hpp"

#include <engine/constants.hpp>

#include <engine/board.hpp>
#include <engine/move.hpp>

#include <engine/zobrist.hpp>
#include <engine/movegen/tables.hpp>
#include <engine/movegen/movegen.hpp>

void setup()
{
    magics::init();
    tables::init();
    zobrist::init();
}

int perft(Board &board, int depth)
{
    if (depth == 0)
    {
        return 1;
    }

    int nodes = 0;
    for (const Move &move : movegen::generate_pseudo_legal_moves(board))
    {
        Board::GameState board_info = board.get_state();
        board.make_move(move);
        REQUIRE(board.calculate_hash_key() == board.get_hash_key());
        int king_sq = bitboard::bit_scan_forward(board.get_pieces(board.get_opponent(), KING));
        int attacker_side = board.get_side_to_move();
        if (!board.is_square_attacked(king_sq, attacker_side))
        {
            nodes += perft(board, depth - 1);
        }
        board.unmake_move(move, board_info);
        REQUIRE(board.calculate_hash_key() == board.get_hash_key());
    }

    return nodes;
}

TEST_CASE("hash_key")
{
    setup();

    Board board = Board();
    SECTION("fen hash key")
    {
        board.set_from_fen("r3k2r/p1ppqpb1/bn2pnp1/3PN3/1p2P3/2N2Q1p/PPPBBPPP/R3K2R", "w", "KQkq", "-", "0", "1");
        REQUIRE(board.calculate_hash_key() == board.get_hash_key());
    }

    SECTION("moves hash key")
    {
        board.set_from_fen("r3k2r/p1ppqpb1/bn2pnp1/3PN3/1p2P3/2N2Q1p/PPPBBPPP/R3K2R", "w", "KQkq", "-", "0", "1");
        Move move = Move(D5, D6, PAWN, EMPTY_PIECE, EMPTY_PIECE, false, false, false);
        Board::GameState state = board.get_state();
        board.make_move(move);
        REQUIRE(board.calculate_hash_key() == board.get_hash_key());
        REQUIRE(zobrist::generate_hash_key(board) == board.get_hash_key());
        board.unmake_move(move, state);
        REQUIRE(board.calculate_hash_key() == board.get_hash_key());
    }

    SECTION("capture hash key")
    {
        board.set_from_fen("r3k2r/p1ppqpb1/bn2pnp1/3PN3/1p2P3/2N2Q1p/PPPBBPPP/R3K2R", "w", "KQkq", "-", "0", "1");
        Move move = Move(F3, F6, QUEEN, KNIGHT, EMPTY_PIECE, false, false, false);
        Board::GameState state = board.get_state();
        board.make_move(move);
        REQUIRE(board.calculate_hash_key() == board.get_hash_key());
        REQUIRE(zobrist::generate_hash_key(board) == board.get_hash_key());
        board.unmake_move(move, state);
        REQUIRE(board.calculate_hash_key() == board.get_hash_key());
    }

    SECTION("double push hash key")
    {
        board.set_from_fen("r3k2r/p1ppqpb1/bn2pnp1/3PN3/1p2P3/2N2Q1p/PPPBBPPP/R3K2R", "w", "KQkq", "-", "0", "1");
        Move move = Move(A2, A4, PAWN, EMPTY_PIECE, EMPTY_PIECE, true, false, false);
        Board::GameState state = board.get_state();
        board.make_move(move);
        REQUIRE(board.calculate_hash_key() == board.get_hash_key());
        REQUIRE(zobrist::generate_hash_key(board) == board.get_hash_key());
        board.unmake_move(move, state);
        REQUIRE(board.calculate_hash_key() == board.get_hash_key());
    }

    SECTION("promotion hash key")
    {
        board.set_from_fen("r3k2r/p1ppqpb1/bn2pnp1/3PN3/1p2P3/2N2Q2/PPPBBP1p/R3K1P1", "b", "KQkq", "-", "0", "1");
        Move move = Move(H2, H1, PAWN, EMPTY_PIECE, KNIGHT, false, false, false);
        Board::GameState state = board.get_state();
        board.make_move(move);
        REQUIRE(board.calculate_hash_key() == board.get_hash_key());
        REQUIRE(zobrist::generate_hash_key(board) == board.get_hash_key());
        board.unmake_move(move, state);
        REQUIRE(board.calculate_hash_key() == board.get_hash_key());
    }

    SECTION("promotion + capture hash key")
    {
        board.set_from_fen("r3k2r/p1ppqpb1/bn2pnp1/3PN3/1p2P3/2N2Q2/PPPBBP1p/R3K1PR", "b", "KQkq", "-", "0", "1");
        Move move = Move(H2, G1, PAWN, PAWN, QUEEN, false, false, false);
        Board::GameState state = board.get_state();
        board.make_move(move);
        REQUIRE(board.calculate_hash_key() == board.get_hash_key());
        REQUIRE(zobrist::generate_hash_key(board) == board.get_hash_key());
        board.unmake_move(move, state);
        REQUIRE(board.calculate_hash_key() == board.get_hash_key());
    }

    SECTION("en passant hash key")
    {
        board.set_from_fen("rnbqkbnr/3ppppp/8/p1p5/2pP2P1/5N1B/PP2PP1P/RNBQK2R", "b", "KQkq", "d3", "0", "5");
        Move move = Move(C4, D3, PAWN, PAWN, EMPTY_PIECE, false, true, false);
        Board::GameState state = board.get_state();
        board.make_move(move);
        REQUIRE(board.calculate_hash_key() == board.get_hash_key());
        REQUIRE(zobrist::generate_hash_key(board) == board.get_hash_key());
        board.unmake_move(move, state);
        REQUIRE(board.calculate_hash_key() == board.get_hash_key());
    }

    SECTION("perft hash key")
    {
        perft(board, 5);
    }
}