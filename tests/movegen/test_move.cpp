#define CATCH_CONFIG_MAIN

#include "../catch.hpp"

#include "../../src/engine/movegen/move.hpp"

// TODO: Missing tests for getUCI()

TEST_CASE("Test Moves")
{
    SECTION("Impossible Move")
    {
        Move move = Move(0, 63, PAWN, QUEEN, QUEEN, true, false, true);
        REQUIRE(move.getFromSquare() == 0);
        REQUIRE(move.getToSquare() == 63);
        REQUIRE(move.getPiece() == PAWN);
        REQUIRE(move.getCapturedPiece() == QUEEN);
        REQUIRE(move.getPromotedPiece() == QUEEN);
        REQUIRE(move.isDoublePush() == true);
        REQUIRE(move.isEnPassant() == false);
        REQUIRE(move.isCastle() == true);
        REQUIRE(move.isCapture() == true);
        REQUIRE(move.score() == 505);
    }

    SECTION("Pawn Takes King")
    {
        Move move = Move(0, 1, PAWN, KING, EMPTY, false, false, false);
        REQUIRE(move.getFromSquare() == 0);
        REQUIRE(move.getToSquare() == 1);
        REQUIRE(move.getPiece() == PAWN);
        REQUIRE(move.getCapturedPiece() == KING);
        REQUIRE(move.getPromotedPiece() == EMPTY);
        REQUIRE(move.isDoublePush() == false);
        REQUIRE(move.isEnPassant() == false);
        REQUIRE(move.isCastle() == false);
        REQUIRE(move.isCapture() == true);
        REQUIRE(move.score() == 605);
    }

    SECTION("Pawn Takes Queen")
    {
        Move move = Move(0, 1, PAWN, QUEEN, EMPTY, false, false, false);
        REQUIRE(move.getFromSquare() == 0);
        REQUIRE(move.getToSquare() == 1);
        REQUIRE(move.getPiece() == PAWN);
        REQUIRE(move.getCapturedPiece() == QUEEN);
        REQUIRE(move.getPromotedPiece() == EMPTY);
        REQUIRE(move.isDoublePush() == false);
        REQUIRE(move.isEnPassant() == false);
        REQUIRE(move.isCastle() == false);
        REQUIRE(move.isCapture() == true);
        REQUIRE(move.score() == 505);
    }

    SECTION("Pawn Takes Rook")
    {
        Move move = Move(0, 1, PAWN, ROOK, EMPTY, false, false, false);
        REQUIRE(move.getFromSquare() == 0);
        REQUIRE(move.getToSquare() == 1);
        REQUIRE(move.getPiece() == PAWN);
        REQUIRE(move.getCapturedPiece() == ROOK);
        REQUIRE(move.getPromotedPiece() == EMPTY);
        REQUIRE(move.isDoublePush() == false);
        REQUIRE(move.isEnPassant() == false);
        REQUIRE(move.isCastle() == false);
        REQUIRE(move.isCapture() == true);
        REQUIRE(move.score() == 405);
    }

    SECTION("Pawn Takes Bishop")
    {
        Move move = Move(0, 1, PAWN, BISHOP, EMPTY, false, false, false);
        REQUIRE(move.getFromSquare() == 0);
        REQUIRE(move.getToSquare() == 1);
        REQUIRE(move.getPiece() == PAWN);
        REQUIRE(move.getCapturedPiece() == BISHOP);
        REQUIRE(move.getPromotedPiece() == EMPTY);
        REQUIRE(move.isDoublePush() == false);
        REQUIRE(move.isEnPassant() == false);
        REQUIRE(move.isCastle() == false);
        REQUIRE(move.isCapture() == true);
        REQUIRE(move.score() == 305);
    }

    SECTION("Pawn Takes Knight")
    {
        Move move = Move(0, 1, PAWN, KNIGHT, EMPTY, false, false, false);
        REQUIRE(move.getFromSquare() == 0);
        REQUIRE(move.getToSquare() == 1);
        REQUIRE(move.getPiece() == PAWN);
        REQUIRE(move.getCapturedPiece() == KNIGHT);
        REQUIRE(move.getPromotedPiece() == EMPTY);
        REQUIRE(move.isDoublePush() == false);
        REQUIRE(move.isEnPassant() == false);
        REQUIRE(move.isCastle() == false);
        REQUIRE(move.isCapture() == true);
        REQUIRE(move.score() == 205);
    }

    SECTION("Pawn Takes Pawn")
    {
        Move move = Move(0, 1, PAWN, PAWN, EMPTY, false, false, false);
        REQUIRE(move.getFromSquare() == 0);
        REQUIRE(move.getToSquare() == 1);
        REQUIRE(move.getPiece() == PAWN);
        REQUIRE(move.getCapturedPiece() == PAWN);
        REQUIRE(move.getPromotedPiece() == EMPTY);
        REQUIRE(move.isDoublePush() == false);
        REQUIRE(move.isEnPassant() == false);
        REQUIRE(move.isCastle() == false);
        REQUIRE(move.isCapture() == true);
        REQUIRE(move.score() == 105);
    }
}