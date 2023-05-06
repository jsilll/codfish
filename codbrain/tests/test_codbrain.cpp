#include <catch2/catch_test_macros.hpp>

#include <codbrain/codbrain.hpp>

using namespace codchess;
using namespace codbrain;

TEST_CASE("Checkmate in one move") {
    auto mp = Brain(3);

    SECTION("1k6/p6p/K6P/8/8/8/8/1q4q1 b - - 0 1") {
        mp.board().SetFromFen("1k6/p6p/K6P/8/8/8/8/1q4q1 b - - 0 1");
        const auto result = mp.FindBestMove();
        Move best_move = result.pv[0];
        REQUIRE(best_move.ToString() == "g1b6");
    }

    SECTION("k6r/8/8/8/8/8/8/2bPKPb1 b - - 0 1") {
        mp.board().SetFromFen("k6r/8/8/8/8/8/8/2bPKPb1 b - - 0 1");
        const auto result = mp.FindBestMove();
        Move best_move = result.pv[0];
        REQUIRE(best_move.ToString() == "h8e8");
    }
}
