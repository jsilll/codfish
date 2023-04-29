#include <catch2/benchmark/catch_benchmark.hpp>
#include <catch2/catch_test_macros.hpp>

#include <codchess/perft.hpp>

TEST_CASE("Benchmark Perft", "[benchmark]") {
    codchess::Init();
    using namespace codchess;

    std::uint64_t nodes{0};

    SECTION("Starting Board") {
        auto board = Board();
        BENCHMARK("Depth 6") { return nodes = perft::Perft(board, 6); };
        REQUIRE(nodes == 119060324);
        BENCHMARK("Depth 7") { return nodes = perft::Perft(board, 7); };
        REQUIRE(nodes == 3195901860);
    }

    SECTION("KiwiPete Board") {
        auto board = Board("r3k2r/p1ppqpb1/bn2pnp1/3PN3/1p2P3/2N2Q1p/PPPBBPPP/"
                           "R3K2R w KQkq - 0 1");
        BENCHMARK("Depth 6") { return nodes = perft::Perft(board, 6); };
        REQUIRE(nodes == 8031647685);
    }

    SECTION("Chess Programming Wiki Board 3") {
        auto board = Board("8/2p5/3p4/KP5r/1R3p1k/8/4P1P1/8 w - - 0 1");
        BENCHMARK("Depth 7") { return nodes = perft::Perft(board, 7); };
        REQUIRE(nodes == 178633661);
    }

    SECTION("Chess Programming Wiki Board 4") {
        auto board = Board(
            "r3k2r/Pppp1ppp/1b3nbN/nP6/BBP1P3/q4N2/Pp1P2PP/R2Q1RK1 w kq - 0 1");
        BENCHMARK("Depth 6") { return nodes = perft::Perft(board, 6); };
        REQUIRE(nodes == 706045033);
    }

    SECTION("Chess Programming Wiki Board 5") {
        auto board =
            Board("rnbq1k1r/pp1Pbppp/2p5/8/2B5/8/PPP1NnPP/RNBQK2R w KQ - 1 8");
        BENCHMARK("Depth 5") { return nodes = perft::Perft(board, 5); };
        REQUIRE(nodes == 89941194);
    }

    SECTION("Chess Programming Wiki Board 6") {
        auto board = Board("r4rk1/1pp1qppp/p1np1n2/2b1p1B1/2B1P1b1/P1NP1N2/"
                           "1PP1QPPP/R4RK1 w - - 0 10");
        BENCHMARK("Depth 6") { return nodes = perft::Perft(board, 6); };
        REQUIRE(nodes == 6923051137);
    }
}
