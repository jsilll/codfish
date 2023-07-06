#include <catch2/catch_test_macros.hpp>

#include <codchess/perft.hpp>

using namespace cod;

using chess::Board;

TEST_CASE("PerftUnmake doesn't modify the board", "[perft]") {
    chess::Init();

    SECTION("Starting Board") {
        auto board = Board();
        const auto backup = board.GetStateBackup();
        chess::perft::PerftCopy(board, 5);
        REQUIRE(board.hash() == backup.hash);
        chess::perft::PerftUnmake(board, 5);
        REQUIRE(board.hash() == backup.hash);
    }
}

TEST_CASE("PerftUnmake visits the correct number of boards", "[perft]") {
    chess::Init();

    SECTION("Starting Board") {
        auto board = Board();

        REQUIRE(chess::perft::PerftCopy(board, 1) == 20);
        REQUIRE(chess::perft::PerftCopy(board, 2) == 400);
        REQUIRE(chess::perft::PerftCopy(board, 3) == 8902);
        REQUIRE(chess::perft::PerftCopy(board, 4) == 197281);
        REQUIRE(chess::perft::PerftCopy(board, 5) == 4865609);
        REQUIRE(chess::perft::PerftCopy(board, 6) == 119060324);

        REQUIRE(chess::perft::PerftUnmake(board, 1) == 20);
        REQUIRE(chess::perft::PerftUnmake(board, 2) == 400);
        REQUIRE(chess::perft::PerftUnmake(board, 3) == 8902);
        REQUIRE(chess::perft::PerftUnmake(board, 4) == 197281);
        REQUIRE(chess::perft::PerftUnmake(board, 5) == 4865609);
        REQUIRE(chess::perft::PerftUnmake(board, 6) == 119060324);
    }

    SECTION("KiwiPete Board") {
        auto board = Board("r3k2r/p1ppqpb1/bn2pnp1/3PN3/1p2P3/2N2Q1p/PPPBBPPP/"
                           "R3K2R w KQkq - 0 1");

        REQUIRE(chess::perft::PerftCopy(board, 1) == 48);
        REQUIRE(chess::perft::PerftCopy(board, 2) == 2039);
        REQUIRE(chess::perft::PerftCopy(board, 3) == 97862);
        REQUIRE(chess::perft::PerftCopy(board, 4) == 4085603);
        REQUIRE(chess::perft::PerftCopy(board, 5) == 193690690);

        REQUIRE(chess::perft::PerftUnmake(board, 1) == 48);
        REQUIRE(chess::perft::PerftUnmake(board, 2) == 2039);
        REQUIRE(chess::perft::PerftUnmake(board, 3) == 97862);
        REQUIRE(chess::perft::PerftUnmake(board, 4) == 4085603);
        REQUIRE(chess::perft::PerftUnmake(board, 5) == 193690690);
    }

    SECTION("Chess Programming Wiki Board 3") {
        auto board = Board("8/2p5/3p4/KP5r/1R3p1k/8/4P1P1/8 w - - 0 1");

        REQUIRE(chess::perft::PerftCopy(board, 1) == 14);
        REQUIRE(chess::perft::PerftCopy(board, 2) == 191);
        REQUIRE(chess::perft::PerftCopy(board, 3) == 2812);
        REQUIRE(chess::perft::PerftCopy(board, 4) == 43238);
        REQUIRE(chess::perft::PerftCopy(board, 5) == 674624);
        REQUIRE(chess::perft::PerftCopy(board, 6) == 11030083);

        REQUIRE(chess::perft::PerftUnmake(board, 1) == 14);
        REQUIRE(chess::perft::PerftUnmake(board, 2) == 191);
        REQUIRE(chess::perft::PerftUnmake(board, 3) == 2812);
        REQUIRE(chess::perft::PerftUnmake(board, 4) == 43238);
        REQUIRE(chess::perft::PerftUnmake(board, 5) == 674624);
        REQUIRE(chess::perft::PerftUnmake(board, 6) == 11030083);
    }

    SECTION("Chess Programming Wiki Board 4") {
        auto board = Board(
            "r3k2r/Pppp1ppp/1b3nbN/nP6/BBP1P3/q4N2/Pp1P2PP/R2Q1RK1 w kq - 0 1");

        REQUIRE(chess::perft::PerftCopy(board, 1) == 6);
        REQUIRE(chess::perft::PerftCopy(board, 2) == 264);
        REQUIRE(chess::perft::PerftCopy(board, 3) == 9467);
        REQUIRE(chess::perft::PerftCopy(board, 4) == 422333);
        REQUIRE(chess::perft::PerftCopy(board, 5) == 15833292);

        REQUIRE(chess::perft::PerftUnmake(board, 1) == 6);
        REQUIRE(chess::perft::PerftUnmake(board, 2) == 264);
        REQUIRE(chess::perft::PerftUnmake(board, 3) == 9467);
        REQUIRE(chess::perft::PerftUnmake(board, 4) == 422333);
        REQUIRE(chess::perft::PerftUnmake(board, 5) == 15833292);
    }

    SECTION("Chess Programming Wiki Board 5") {
        auto board =
            Board("rnbq1k1r/pp1Pbppp/2p5/8/2B5/8/PPP1NnPP/RNBQK2R w KQ - 1 8");

        REQUIRE(chess::perft::PerftCopy(board, 1) == 44);
        REQUIRE(chess::perft::PerftCopy(board, 2) == 1486);
        REQUIRE(chess::perft::PerftCopy(board, 3) == 62379);
        REQUIRE(chess::perft::PerftCopy(board, 4) == 2103487);
        REQUIRE(chess::perft::PerftCopy(board, 5) == 89941194);

        REQUIRE(chess::perft::PerftUnmake(board, 1) == 44);
        REQUIRE(chess::perft::PerftUnmake(board, 2) == 1486);
        REQUIRE(chess::perft::PerftUnmake(board, 3) == 62379);
        REQUIRE(chess::perft::PerftUnmake(board, 4) == 2103487);
        REQUIRE(chess::perft::PerftUnmake(board, 5) == 89941194);
    }

    SECTION("Chess Programming Wiki Board 6") {
        auto board = Board("r4rk1/1pp1qppp/p1np1n2/2b1p1B1/2B1P1b1/P1NP1N2/"
                           "1PP1QPPP/R4RK1 w - - 0 10");

        REQUIRE(chess::perft::PerftCopy(board, 1) == 46);
        REQUIRE(chess::perft::PerftCopy(board, 2) == 2079);
        REQUIRE(chess::perft::PerftCopy(board, 3) == 89890);
        REQUIRE(chess::perft::PerftCopy(board, 4) == 3894594);
        REQUIRE(chess::perft::PerftCopy(board, 5) == 164075551);

        REQUIRE(chess::perft::PerftUnmake(board, 1) == 46);
        REQUIRE(chess::perft::PerftUnmake(board, 2) == 2079);
        REQUIRE(chess::perft::PerftUnmake(board, 3) == 89890);
        REQUIRE(chess::perft::PerftUnmake(board, 4) == 3894594);
        REQUIRE(chess::perft::PerftUnmake(board, 5) == 164075551);
    }
}