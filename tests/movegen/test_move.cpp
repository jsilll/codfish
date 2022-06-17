#define CATCH_CONFIG_MAIN

#include "../catch.hpp"

#include "../../src/engine/movegen/move.hpp"

TEST_CASE("Test get piece from square")
{
    SECTION("Pawn Takes King")
    {
        Move move = Move(0, 1, PAWN, KING, 0, true, false, false, false);
        REQUIRE(move.score() == 605);
    }

    SECTION("Pawn Takes Queen")
    {
        Move move = Move(0, 1, PAWN, QUEEN, 0, true, false, false, false);
        REQUIRE(move.score() == 505);
    }

    SECTION("Pawn Takes Rook")
    {
        Move move = Move(0, 1, PAWN, ROOK, 0, true, false, false, false);
        REQUIRE(move.score() == 405);
    }

    SECTION("Pawn Takes Bishop")
    {
        Move move = Move(0, 1, PAWN, BISHOP, 0, true, false, false, false);
        REQUIRE(move.score() == 305);
    }

    SECTION("Pawn Takes Knight")
    {
        Move move = Move(0, 1, PAWN, KNIGHT, 0, true, false, false, false);
        REQUIRE(move.score() == 205);
    }

    SECTION("Pawn Takes Pawn")
    {
        Move move = Move(0, 1, PAWN, PAWN, 0, true, false, false, false);
        REQUIRE(move.score() == 105);
    }
}