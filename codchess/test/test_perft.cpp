#include <catch2/catch_test_macros.hpp>

#include <codchess/perft.hpp>

using namespace codchess;

TEST_CASE("PerftUnmake doesn't modify the board", "[perft]") {
    ::codchess::Init();

    SECTION("Starting Board") {
        auto board = Board();
        const auto backup = board.GetStateBackup();
        perft::PerftCopy(board, 5);
        REQUIRE(board.hash() == backup.hash);
        perft::PerftUnmake(board, 5);
        REQUIRE(board.hash() == backup.hash);
    }
}

TEST_CASE("PerftUnmake visits the correct number of boards", "[perft]") {
    ::codchess::Init();

    SECTION("Starting Board") {
        auto board = Board();
        REQUIRE(perft::PerftCopy(board, 1) == 20);
        REQUIRE(perft::PerftCopy(board, 2) == 400);
        REQUIRE(perft::PerftCopy(board, 3) == 8902);
        REQUIRE(perft::PerftCopy(board, 4) == 197281);
        REQUIRE(perft::PerftCopy(board, 5) == 4865609);
        REQUIRE(perft::PerftCopy(board, 6) == 119060324);
        REQUIRE(perft::PerftUnmake(board, 1) == 20);
        REQUIRE(perft::PerftUnmake(board, 2) == 400);
        REQUIRE(perft::PerftUnmake(board, 3) == 8902);
        REQUIRE(perft::PerftUnmake(board, 4) == 197281);
        REQUIRE(perft::PerftUnmake(board, 5) == 4865609);
        REQUIRE(perft::PerftUnmake(board, 6) == 119060324);
    }

    SECTION("KiwiPete Board") {
        auto board = Board("r3k2r/p1ppqpb1/bn2pnp1/3PN3/1p2P3/2N2Q1p/PPPBBPPP/"
                           "R3K2R w KQkq - 0 1");
        REQUIRE(perft::PerftCopy(board, 1) == 48);
        REQUIRE(perft::PerftCopy(board, 2) == 2039);
        REQUIRE(perft::PerftCopy(board, 3) == 97862);
        REQUIRE(perft::PerftCopy(board, 4) == 4085603);
        REQUIRE(perft::PerftCopy(board, 5) == 193690690);
        REQUIRE(perft::PerftUnmake(board, 1) == 48);
        REQUIRE(perft::PerftUnmake(board, 2) == 2039);
        REQUIRE(perft::PerftUnmake(board, 3) == 97862);
        REQUIRE(perft::PerftUnmake(board, 4) == 4085603);
        REQUIRE(perft::PerftUnmake(board, 5) == 193690690);
    }

    SECTION("Chess Programming Wiki Board 3") {
        auto board = Board("8/2p5/3p4/KP5r/1R3p1k/8/4P1P1/8 w - - 0 1");
        REQUIRE(perft::PerftCopy(board, 1) == 14);
        REQUIRE(perft::PerftCopy(board, 2) == 191);
        REQUIRE(perft::PerftCopy(board, 3) == 2812);
        REQUIRE(perft::PerftCopy(board, 4) == 43238);
        REQUIRE(perft::PerftCopy(board, 5) == 674624);
        REQUIRE(perft::PerftCopy(board, 6) == 11030083);
        REQUIRE(perft::PerftUnmake(board, 1) == 14);
        REQUIRE(perft::PerftUnmake(board, 2) == 191);
        REQUIRE(perft::PerftUnmake(board, 3) == 2812);
        REQUIRE(perft::PerftUnmake(board, 4) == 43238);
        REQUIRE(perft::PerftUnmake(board, 5) == 674624);
        REQUIRE(perft::PerftUnmake(board, 6) == 11030083);
    }

    SECTION("Chess Programming Wiki Board 4") {
        auto board = Board(
            "r3k2r/Pppp1ppp/1b3nbN/nP6/BBP1P3/q4N2/Pp1P2PP/R2Q1RK1 w kq - 0 1");
        REQUIRE(perft::PerftCopy(board, 1) == 6);
        REQUIRE(perft::PerftCopy(board, 2) == 264);
        REQUIRE(perft::PerftCopy(board, 3) == 9467);
        REQUIRE(perft::PerftCopy(board, 4) == 422333);
        REQUIRE(perft::PerftCopy(board, 5) == 15833292);
        REQUIRE(perft::PerftUnmake(board, 1) == 6);
        REQUIRE(perft::PerftUnmake(board, 2) == 264);
        REQUIRE(perft::PerftUnmake(board, 3) == 9467);
        REQUIRE(perft::PerftUnmake(board, 4) == 422333);
        REQUIRE(perft::PerftUnmake(board, 5) == 15833292);
    }

    SECTION("Chess Programming Wiki Board 5") {
        auto board =
            Board("rnbq1k1r/pp1Pbppp/2p5/8/2B5/8/PPP1NnPP/RNBQK2R w KQ - 1 8");
        REQUIRE(perft::PerftCopy(board, 1) == 44);
        REQUIRE(perft::PerftCopy(board, 2) == 1486);
        REQUIRE(perft::PerftCopy(board, 3) == 62379);
        REQUIRE(perft::PerftCopy(board, 4) == 2103487);
        REQUIRE(perft::PerftCopy(board, 5) == 89941194);
        REQUIRE(perft::PerftUnmake(board, 1) == 44);
        REQUIRE(perft::PerftUnmake(board, 2) == 1486);
        REQUIRE(perft::PerftUnmake(board, 3) == 62379);
        REQUIRE(perft::PerftUnmake(board, 4) == 2103487);
        REQUIRE(perft::PerftUnmake(board, 5) == 89941194);
    }

    SECTION("Chess Programming Wiki Board 6") {
        auto board = Board("r4rk1/1pp1qppp/p1np1n2/2b1p1B1/2B1P1b1/P1NP1N2/"
                           "1PP1QPPP/R4RK1 w - - 0 10");
        REQUIRE(perft::PerftCopy(board, 1) == 46);
        REQUIRE(perft::PerftCopy(board, 2) == 2079);
        REQUIRE(perft::PerftCopy(board, 3) == 89890);
        REQUIRE(perft::PerftCopy(board, 4) == 3894594);
        REQUIRE(perft::PerftCopy(board, 5) == 164075551);
        REQUIRE(perft::PerftUnmake(board, 1) == 46);
        REQUIRE(perft::PerftUnmake(board, 2) == 2079);
        REQUIRE(perft::PerftUnmake(board, 3) == 89890);
        REQUIRE(perft::PerftUnmake(board, 4) == 3894594);
        REQUIRE(perft::PerftUnmake(board, 5) == 164075551);
    }
}