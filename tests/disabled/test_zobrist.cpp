#include "catch2/catch_test_macros.hpp"

// #include <codlib/codlib.hpp>
// #include <codlib/perft.hpp>
// #include <codlib/zobrist.hpp>
//
// TEST_CASE("hash_key")
// {
//   codlib::init();
//
//   Board board = Board();
//
//   SECTION("fen hash key")
//   {
//     board.set_from_fen("r3k2r/p1ppqpb1/bn2pnp1/3PN3/1p2P3/2N2Q1p/PPPBBPPP/R3K2R", "w", "KQkq", "-", "0", "1");
//     REQUIRE(board.get_hash_key() == zobrist::generate_hash_key(board));
//   }
//
//   SECTION("moves hash key")
//   {
//     board.set_from_fen("r3k2r/p1ppqpb1/bn2pnp1/3PN3/1p2P3/2N2Q1p/PPPBBPPP/R3K2R", "w", "KQkq", "-", "0", "1");
//     Move move = Move(D5, D6, PAWN, EMPTY_PIECE, EMPTY_PIECE, false, false, false);
//     Board::GameState state = board.get_state();
//     board.make(move);
//     REQUIRE(board.get_hash_key() == zobrist::generate_hash_key(board));
//     board.unmake(move, state);
//     REQUIRE(board.get_hash_key() == state.hash_key);
//   }
//
//   SECTION("capture hash key")
//   {
//     board.set_from_fen("r3k2r/p1ppqpb1/bn2pnp1/3PN3/1p2P3/2N2Q1p/PPPBBPPP/R3K2R", "w", "KQkq", "-", "0", "1");
//     Move move = Move(F3, F6, QUEEN, KNIGHT, EMPTY_PIECE, false, false, false);
//     Board::GameState state = board.get_state();
//     board.make(move);
//     REQUIRE(board.get_hash_key() == zobrist::generate_hash_key(board));
//     board.unmake(move, state);
//     REQUIRE(board.get_hash_key() == state.hash_key);
//   }
//
//   SECTION("double push hash key")
//   {
//     board.set_from_fen("r3k2r/p1ppqpb1/bn2pnp1/3PN3/1p2P3/2N2Q1p/PPPBBPPP/R3K2R", "w", "KQkq", "-", "0", "1");
//     Move move = Move(A2, A4, PAWN, EMPTY_PIECE, EMPTY_PIECE, true, false, false);
//     Board::GameState state = board.get_state();
//     board.make(move);
//     REQUIRE(board.get_hash_key() == zobrist::generate_hash_key(board));
//     board.unmake(move, state);
//     REQUIRE(board.get_hash_key() == state.hash_key);
//   }
//
//   SECTION("promotion hash key")
//   {
//     board.set_from_fen("r3k2r/p1ppqpb1/bn2pnp1/3PN3/1p2P3/2N2Q2/PPPBBP1p/R3K1P1", "b", "KQkq", "-", "0", "1");
//     Move move = Move(H2, H1, PAWN, EMPTY_PIECE, KNIGHT, false, false, false);
//     Board::GameState state = board.get_state();
//     board.make(move);
//     REQUIRE(board.get_hash_key() == zobrist::generate_hash_key(board));
//     board.unmake(move, state);
//     REQUIRE(board.get_hash_key() == state.hash_key);
//   }
//
//   SECTION("promotion + capture hash key")
//   {
//     board.set_from_fen("r3k2r/p1ppqpb1/bn2pnp1/3PN3/1p2P3/2N2Q2/PPPBBP1p/R3K1PR", "b", "KQkq", "-", "0", "1");
//     Move move = Move(H2, G1, PAWN, PAWN, QUEEN, false, false, false);
//     Board::GameState state = board.get_state();
//     board.make(move);
//     REQUIRE(board.get_hash_key() == zobrist::generate_hash_key(board));
//     board.unmake(move, state);
//     REQUIRE(board.get_hash_key() == state.hash_key);
//   }
//
//   SECTION("en passant hash key")
//   {
//     board.set_from_fen("rnbqkbnr/3ppppp/8/p1p5/2pP2P1/5N1B/PP2PP1P/RNBQK2R", "b", "KQkq", "d3", "0", "5");
//     Move move = Move(C4, D3, PAWN, PAWN, EMPTY_PIECE, false, true, false);
//     Board::GameState state = board.get_state();
//     board.make(move);
//     REQUIRE(board.get_hash_key() == zobrist::generate_hash_key(board));
//     board.unmake(move, state);
//     REQUIRE(board.get_hash_key() == state.hash_key);
//   }
//
//   SECTION("perft hash key")
//   {
//     Board::GameState state = board.get_state();
//     perft::perft(board, 5);
//     REQUIRE(board.get_hash_key() == state.hash_key);
//   }
// }