#include <catch2/catch_test_macros.hpp>

#include <codbrain/codbrain.hpp>

using namespace codchess;
using namespace codbrain;

TEST_CASE("Checkmate in one move") {
    auto board = Board();
    auto mp = MovePicker(board);
    mp.set_max_depth(3);

    SECTION("1k6/p6p/K6P/8/8/8/8/1q4q1 b - - 0 1") {
        board.SetFromFen("1k6/p6p/K6P/8/8/8/8/1q4q1 b - - 0 1");
        MovePicker::SearchResult result = mp.find_best_move();
        Move best_move = result.pv[0];
        REQUIRE(best_move.Uci() == "g1b6");
    }

    SECTION("k6r/8/8/8/8/8/8/2bPKPb1 b - - 0 1") {
        board.SetFromFen("k6r/8/8/8/8/8/8/2bPKPb1 b - - 0 1");
        MovePicker::SearchResult result = mp.find_best_move();
        Move best_move = result.pv[0];
        REQUIRE(best_move.Uci() == "h8e8");
    }
}