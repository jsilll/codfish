#define CATCH_CONFIG_MAIN
#include "catch.hpp"

#include "../src/engine/defs.hpp"
#include "../src/engine/movegen/magics.hpp"
#include "../src/engine/movegen/tables.hpp"
#include "../src/engine/movegen/board.hpp"
#include "../src/engine/movegen/move.hpp"
#include "../src/engine/movepicker/movepicker.hpp"

void setup()
{
    magics::init();
    tables::init();
}

TEST_CASE("Checkmate in one move")
{
    setup();

    Board board = Board();
    MovePicker ai = MovePicker(board);
    ai.setDepth(3);

    SECTION("1k6/p6p/K6P/8/8/8/8/1q4q1 b - - 0 1")
    {
        board.setFromFen("1k6/p6p/K6P/8/8/8/8/1q4q1", "b", "-", "-", "0", "1");
        MovePicker::SearchResult result = ai.findBestMove();
        Move best_move = Move(result.best_move_encoded);
        REQUIRE(best_move.getUCI() == "b1b6");
    }

    SECTION("k6r/8/8/8/8/8/8/2bPKPb1 b - - 0 1")
    {
        board.setFromFen("k6r/8/8/8/8/8/8/2bPKPb1", "b", "-", "-", "0", "1");
        MovePicker::SearchResult result = ai.findBestMove();
        Move best_move = Move(result.best_move_encoded);
        REQUIRE(best_move.getUCI() == "h8e8");
    }
}
