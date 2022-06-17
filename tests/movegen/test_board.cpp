#define CATCH_CONFIG_MAIN

#include "../catch.hpp"

#include "../../src/engine/defs.hpp"
#include "../../src/engine/movegen/magics.hpp"
#include "../../src/engine/movegen/tables.hpp"
#include "../../src/engine/movegen/board.hpp"
// #include "../../src/engine/movegen/perft.hpp"

void setup()
{
    magics::init();
    tables::init();
}

TEST_CASE("Test get piece from square")
{
    setup();

    SECTION("Position 1")
    {
        Board board = Board();
        board.setFromFen("r1b1kb1r/ppp1p2p/5pp1/7K/4pPn1/8/PP1P2nP/RNB2qNR", "w", "kq", "-", "0", "14");
        REQUIRE(board.getPieceFromSquare(A8).type == ROOK);
        REQUIRE(board.getPieceFromSquare(G6).type == PAWN);
        REQUIRE(board.getPieceFromSquare(C1).type == BISHOP);
        REQUIRE(board.getPieceFromSquare(H5).type == KING);
        REQUIRE(board.getPieceFromSquare(G2).type == KNIGHT);
        REQUIRE(board.getPieceFromSquare(F1).type == QUEEN);
        REQUIRE(board.getPieceFromSquare(D4).type == EMPTY);
    }
}