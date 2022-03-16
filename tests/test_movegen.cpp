#define CATCH_CONFIG_MAIN
#include "catch.hpp"
#include "../src/defs.hpp"
#include "../src/magics.hpp"
#include "../src/tables.hpp"
#include "../src/board.hpp"

TEST_CASE("Initial Board")
{
  Magics::init();
  Tables::init();
  Board board = Board();
  REQUIRE(board.getSideToMove() == WHITE);
  REQUIRE(board.getLegalMovesUCI().size() == 20);
  // SECTION("WIP")
  // {
  // }
}