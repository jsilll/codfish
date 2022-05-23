#define CATCH_CONFIG_MAIN
#include "catch.h"

#include "../src/defs.h"
#include "../src/magics.h"
#include "../src/tables.h"
#include "../src/board.h"
#include "../src/move.h"
#include "../src/ai.h"

void setup()
{
    magics::init();
    tables::init();
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
        AI::SearchResult result = ai.findBestMove();
        Move best_move = Move(result.best_move_encoded);
        REQUIRE(best_move.getUCI() == "b1b6");
    }

    SECTION("k6r/8/8/8/8/8/8/2bPKPb1 b - - 0 1")
    {
        board.setFromFen("k6r/8/8/8/8/8/8/2bPKPb1", "b", "-", "-", "0", "1");
        AI::SearchResult result = ai.findBestMove();
        Move best_move = Move(result.best_move_encoded);
        REQUIRE(best_move.getUCI() == "h8e8");
    }
}
