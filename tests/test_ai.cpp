#define CATCH_CONFIG_MAIN
#include "catch.hpp"

#include "../src/defs.hpp"
#include "../src/magics.hpp"
#include "../src/tables.hpp"
#include "../src/board.hpp"
#include "../src/move.hpp"
#include "../src/ai.hpp"

void setup()
{
    Magics::init();
    Tables::init();
}

TEST_CASE("Checkmate in one move")
{
    setup();

    Board board = Board();
    AI ai = AI(board);
    ai.setDepth(3);

    SECTION("1k6/p6p/K6P/8/8/8/8/1q4q1 b - - 0 1")
    {
        board.setFromFen("1k6/p6p/K6P/8/8/8/8/1q4q1", "b", "-", "-", "0", "1");
        AI::SearchResult result = ai.find_best_move();
        Move best_move = Move(result.best_move_encoded);
        REQUIRE(best_move.getUCI() == "b1b6");
    }

    SECTION("k6r/8/8/8/8/8/8/2bPKPb1 b - - 0 1")
    {
        board.setFromFen("k6r/8/8/8/8/8/8/2bPKPb1", "b", "-", "-", "0", "1");
        AI::SearchResult result = ai.find_best_move();
        Move best_move = Move(result.best_move_encoded);
        REQUIRE(best_move.getUCI() == "h8e8");
    }
}
