#define CATCH_CONFIG_MAIN

#include "../catch.hpp"

#include <codlib/base.hpp>

#include <codlib/board.hpp>
#include <codlib/move.hpp>
#include <codlib/movepicker/movepicker.hpp>

#include <codlib/movegen/magics.hpp>
#include <codlib/movegen/tables.hpp>

#include "codlib/zobrist.hpp"
#include <codlib/movepicker/eval.hpp>

void setup()
{
    magics::init();
    tables::init();
    eval::init();
    zobrist::init();
}

TEST_CASE("Checkmate in one move")
{
    setup();

    Board board = Board();
    MovePicker ai = MovePicker(board);
    ai.set_max_depth(3);

    SECTION("1k6/p6p/K6P/8/8/8/8/1q4q1 b - - 0 1")
    {
        board.set_from_fen("1k6/p6p/K6P/8/8/8/8/1q4q1", "b", "-", "-", "0", "1");
        MovePicker::SearchResult result = ai.find_best_move();
        Move best_move = result.pv[0];
        REQUIRE(best_move.get_uci() == "g1b6");
    }

    SECTION("k6r/8/8/8/8/8/8/2bPKPb1 b - - 0 1")
    {
        board.set_from_fen("k6r/8/8/8/8/8/8/2bPKPb1", "b", "-", "-", "0", "1");
        MovePicker::SearchResult result = ai.find_best_move();
        Move best_move = result.pv[0];
        REQUIRE(best_move.get_uci() == "h8e8");
    }
}
