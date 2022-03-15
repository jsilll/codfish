#define CATCH_CONFIG_MAIN
#include "catch.hpp"
#include "../src/defs.hpp"
#include "../src/board.hpp"

TEST_CASE("Initial Board") {
  Board board = Board();
  REQUIRE(board.getSideToMove()==WHITE);
  REQUIRE(board.getLegalMovesUCI().size()==20);
}