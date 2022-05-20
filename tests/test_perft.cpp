#define CATCH_CONFIG_MAIN
#include "catch.hpp"

#include "../src/magics.hpp"
#include "../src/tables.hpp"
#include "../src/board.hpp"
#include "../src/perft.hpp"

void setup()
{
    Magics::init();
    Tables::init();
}

TEST_CASE("Starting Board")
{
    setup();

    Board board = Board();

    SECTION("Perft 0")
    {
        REQUIRE(Perft::perft(board, 0) == 1);
    }

    SECTION("Perft 1")
    {
        REQUIRE(Perft::perft(board, 1) == 20);
    }

    SECTION("Perft 2")
    {
        REQUIRE(Perft::perft(board, 2) == 400);
    }

    SECTION("Perft 3")
    {
        REQUIRE(Perft::perft(board, 3) == 8902);
    }

    SECTION("Perft 4")
    {
        REQUIRE(Perft::perft(board, 4) == 197281);
    }

    SECTION("Perft 5")
    {
        REQUIRE(Perft::perft(board, 5) == 4865609);
    }

    SECTION("Perft 6")
    {
        REQUIRE(Perft::perft(board, 6) == 119060324);
    }
}

TEST_CASE("KiwiPete Board")
{
    setup();

    Board board = Board();
    board.setFromFen("r3k2r/p1ppqpb1/bn2pnp1/3PN3/1p2P3/2N2Q1p/PPPBBPPP/R3K2R", "w", "KQkq", "-", "0", "1");

    SECTION("Perft 0")
    {
        REQUIRE(Perft::perft(board, 0) == 1);
    }

    SECTION("Perft 1")
    {
        REQUIRE(Perft::perft(board, 1) == 48);
    }

    SECTION("Perft 2")
    {
        REQUIRE(Perft::perft(board, 2) == 2039);
    }

    SECTION("Perft 3")
    {
        REQUIRE(Perft::perft(board, 3) == 97862);
    }

    SECTION("Perft 4")
    {
        REQUIRE(Perft::perft(board, 4) == 4085603);
    }

    SECTION("Perft 5")
    {
        REQUIRE(Perft::perft(board, 5) == 193690690);
    }
}

TEST_CASE("Chess Programming Wiki Board 3")
{
    setup();

    Board board = Board();
    board.setFromFen("8/2p5/3p4/KP5r/1R3p1k/8/4P1P1/8", "w", "-", "-", "0", "1");

    SECTION("Perft 0")
    {
        REQUIRE(Perft::perft(board, 0) == 1);
    }

    SECTION("Perft 1")
    {
        REQUIRE(Perft::perft(board, 1) == 14);
    }

    SECTION("Perft 2")
    {
        REQUIRE(Perft::perft(board, 2) == 191);
    }

    SECTION("Perft 3")
    {
        REQUIRE(Perft::perft(board, 3) == 2812);
    }

    SECTION("Perft 4")
    {
        REQUIRE(Perft::perft(board, 4) == 43238);
    }

    SECTION("Perft 5")
    {
        REQUIRE(Perft::perft(board, 5) == 674624);
    }

    SECTION("Perft 6")
    {
        REQUIRE(Perft::perft(board, 6) == 11030083);
    }
}

TEST_CASE("Chess Programming Wiki Board 4")
{
    setup();

    Board board = Board();
    board.setFromFen("r3k2r/Pppp1ppp/1b3nbN/nP6/BBP1P3/q4N2/Pp1P2PP/R2Q1RK1", "w", "kq", "-", "0", "1");

    SECTION("Perft 0")
    {
        REQUIRE(Perft::perft(board, 0) == 1);
    }

    SECTION("Perft 1")
    {
        REQUIRE(Perft::perft(board, 1) == 6);
    }

    SECTION("Perft 2")
    {
        REQUIRE(Perft::perft(board, 2) == 264);
    }

    SECTION("Perft 3")
    {
        REQUIRE(Perft::perft(board, 3) == 9467);
    }

    SECTION("Perft 4")
    {
        REQUIRE(Perft::perft(board, 4) == 422333);
    }

    SECTION("Perft 5")
    {
        REQUIRE(Perft::perft(board, 5) == 15833292);
    }
}

TEST_CASE("Chess Programming Wiki Board 5")
{
    setup();

    Board board = Board();
    board.setFromFen("rnbq1k1r/pp1Pbppp/2p5/8/2B5/8/PPP1NnPP/RNBQK2R", "w", "KQ", "-", "1", "8");

    SECTION("Perft 0")
    {
        REQUIRE(Perft::perft(board, 0) == 1);
    }

    SECTION("Perft 1")
    {
        REQUIRE(Perft::perft(board, 1) == 44);
    }

    SECTION("Perft 2")
    {
        REQUIRE(Perft::perft(board, 2) == 1486);
    }

    SECTION("Perft 3")
    {
        REQUIRE(Perft::perft(board, 3) == 62379);
    }

    SECTION("Perft 4")
    {
        REQUIRE(Perft::perft(board, 4) == 2103487);
    }

    SECTION("Perft 5")
    {
        REQUIRE(Perft::perft(board, 5) == 89941194);
    }
}

TEST_CASE("Chess Programming Wiki Board 6")
{
    setup();

    Board board = Board();
    board.setFromFen("r4rk1/1pp1qppp/p1np1n2/2b1p1B1/2B1P1b1/P1NP1N2/1PP1QPPP/R4RK1", "w", "-", "-", "0", "10");

    SECTION("Perft 0")
    {
        REQUIRE(Perft::perft(board, 0) == 1);
    }

    SECTION("Perft 1")
    {
        REQUIRE(Perft::perft(board, 1) == 46);
    }

    SECTION("Perft 2")
    {
        REQUIRE(Perft::perft(board, 2) == 2079);
    }

    SECTION("Perft 3")
    {
        REQUIRE(Perft::perft(board, 3) == 89890);
    }

    SECTION("Perft 4")
    {
        REQUIRE(Perft::perft(board, 4) == 3894594);
    }

    SECTION("Perft 5")
    {
        REQUIRE(Perft::perft(board, 5) == 164075551);
    }
}