#include <catch2/catch_test_macros.hpp>

#include <codlib/codlib.hpp>
#include <codlib/perft.hpp>

TEST_CASE("Perft doesn't modify the board", "[perft]") {
    codlib::Init();

    SECTION("Starting Board") {
        auto board = Board();
        const auto backup = board.GetStateBackup();
        perft::Perft(board, 5);
        REQUIRE(board.hash_key() == backup.hash_key);
    }
}

TEST_CASE("Perft visits the correct number of boards", "[perft]")
{
    codlib::Init();

    SECTION("Starting Board")
    {
        auto board = Board();
        REQUIRE(perft::Perft(board, 0) == 1);
        REQUIRE(perft::Perft(board, 1) == 20);
        REQUIRE(perft::Perft(board, 2) == 400);
        REQUIRE(perft::Perft(board, 3) == 8902);
        REQUIRE(perft::Perft(board, 4) == 197281);
        REQUIRE(perft::Perft(board, 5) == 4865609);
        REQUIRE(perft::Perft(board, 6) == 119060324);
    }

    SECTION("KiwiPete Board")
    {
        auto board = Board("r3k2r/p1ppqpb1/bn2pnp1/3PN3/1p2P3/2N2Q1p/PPPBBPPP/R3K2R w KQkq - 0 1");
        REQUIRE(perft::Perft(board, 0) == 1);
        REQUIRE(perft::Perft(board, 1) == 48);
        REQUIRE(perft::Perft(board, 2) == 2039);
        REQUIRE(perft::Perft(board, 3) == 97862);
        REQUIRE(perft::Perft(board, 4) == 4085603);
        REQUIRE(perft::Perft(board, 5) == 193690690);
    }

    SECTION("Chess Programming Wiki Board 3")
    {
        auto board = Board("8/2p5/3p4/KP5r/1R3p1k/8/4P1P1/8 w - - 0 1");
        REQUIRE(perft::Perft(board, 0) == 1);
        REQUIRE(perft::Perft(board, 1) == 14);
        REQUIRE(perft::Perft(board, 2) == 191);
        REQUIRE(perft::Perft(board, 3) == 2812);
        REQUIRE(perft::Perft(board, 4) == 43238);
        REQUIRE(perft::Perft(board, 5) == 674624);
        REQUIRE(perft::Perft(board, 6) == 11030083);
    }

    SECTION("Chess Programming Wiki Board 4")
    {
        auto board = Board("r3k2r/Pppp1ppp/1b3nbN/nP6/BBP1P3/q4N2/Pp1P2PP/R2Q1RK1 w kq - 0 1");
        REQUIRE(perft::Perft(board, 0) == 1);
        REQUIRE(perft::Perft(board, 1) == 6);
        REQUIRE(perft::Perft(board, 2) == 264);
        REQUIRE(perft::Perft(board, 3) == 9467);
        REQUIRE(perft::Perft(board, 4) == 422333);
        REQUIRE(perft::Perft(board, 5) == 15833292);
    }

    SECTION("Chess Programming Wiki Board 5")
    {
        auto board = Board("rnbq1k1r/pp1Pbppp/2p5/8/2B5/8/PPP1NnPP/RNBQK2R w KQ - 1 8");
        REQUIRE(perft::Perft(board, 0) == 1);
        REQUIRE(perft::Perft(board, 1) == 44);
        REQUIRE(perft::Perft(board, 2) == 1486);
        REQUIRE(perft::Perft(board, 3) == 62379);
        REQUIRE(perft::Perft(board, 4) == 2103487);
        REQUIRE(perft::Perft(board, 5) == 89941194);
    }

    SECTION("Chess Programming Wiki Board 6")
    {
        auto board = Board("r4rk1/1pp1qppp/p1np1n2/2b1p1B1/2B1P1b1/P1NP1N2/1PP1QPPP/R4RK1 w - - 0 10");
        REQUIRE(perft::Perft(board, 0) == 1);
        REQUIRE(perft::Perft(board, 1) == 46);
        REQUIRE(perft::Perft(board, 2) == 2079);
        REQUIRE(perft::Perft(board, 3) == 89890);
        REQUIRE(perft::Perft(board, 4) == 3894594);
        REQUIRE(perft::Perft(board, 5) == 164075551);
    }
}