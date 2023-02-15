#define CATCH_CONFIG_MAIN

#include "../catch.hpp"

#include <codlib/base.hpp>
#include <codlib/movegen/magics.hpp>
#include <codlib/movegen/tables.hpp>
#include <codlib/board.hpp>

static void setup()
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
        board.set_from_fen("r1b1kb1r/ppp1p2p/5pp1/7K/4pPn1/8/PP1P2nP/RNB2qNR", "w", "kq", "-", "0", "14");
        REQUIRE(board.get_piece_from_square(A8).type == ROOK);
        REQUIRE(board.get_piece_from_square(G6).type == PAWN);
        REQUIRE(board.get_piece_from_square(C1).type == BISHOP);
        REQUIRE(board.get_piece_from_square(H5).type == KING);
        REQUIRE(board.get_piece_from_square(G2).type == KNIGHT);
        REQUIRE(board.get_piece_from_square(F1).type == QUEEN);
        REQUIRE(board.get_piece_from_square(D4).type == EMPTY_PIECE);
    }
}