#include <catch2/catch_test_macros.hpp>

#include <codlib/codlib.hpp>

TEST_CASE("Move Class", "[move]")
{
    codlib::Init();

    SECTION("Impossible Move")
    {
        Move move = Move(0, 63, PAWN, QUEEN, QUEEN, true, false, true);
        REQUIRE(move.FromSquare() == 0);
        REQUIRE(move.ToSquare() == 63);
        REQUIRE(move.MovedPiece() == PAWN);
        REQUIRE(move.CapturedPiece() == QUEEN);
        REQUIRE(move.PromotedPiece() == QUEEN);
        REQUIRE(move.IsDoublePush() == true);
        REQUIRE(move.IsEnPassant() == false);
        REQUIRE(move.IsCastle() == true);
        REQUIRE(move.IsCapture() == true);
        REQUIRE(move.Uci() == "a1h8q");
    }

    SECTION("Pawn Takes King")
    {
        Move move = Move(0, 1, PAWN, KING, EMPTY_PIECE, false, false, false);
        REQUIRE(move.FromSquare() == 0);
        REQUIRE(move.ToSquare() == 1);
        REQUIRE(move.MovedPiece() == PAWN);
        REQUIRE(move.CapturedPiece() == KING);
        REQUIRE(move.PromotedPiece() == EMPTY_PIECE);
        REQUIRE(move.IsPromotion() == false);
        REQUIRE(move.IsDoublePush() == false);
        REQUIRE(move.IsEnPassant() == false);
        REQUIRE(move.IsCastle() == false);
        REQUIRE(move.IsCapture() == true);
        REQUIRE(move.IsPromotion() == false);
        REQUIRE(move.IsPromotion() == false);
        REQUIRE(move.Uci() == "a1b1");
    }

    SECTION("Pawn Takes Queen")
    {
        Move move = Move(0, 1, PAWN, QUEEN, EMPTY_PIECE, false, false, false);
        REQUIRE(move.FromSquare() == 0);
        REQUIRE(move.ToSquare() == 1);
        REQUIRE(move.MovedPiece() == PAWN);
        REQUIRE(move.CapturedPiece() == QUEEN);
        REQUIRE(move.PromotedPiece() == EMPTY_PIECE);
        REQUIRE(move.IsPromotion() == false);
        REQUIRE(move.IsDoublePush() == false);
        REQUIRE(move.IsEnPassant() == false);
        REQUIRE(move.IsCastle() == false);
        REQUIRE(move.IsCapture() == true);
        REQUIRE(move.Uci() == "a1b1");
    }

    SECTION("Pawn Takes Rook")
    {
        Move move = Move(0, 1, PAWN, ROOK, EMPTY_PIECE, false, false, false);
        REQUIRE(move.FromSquare() == 0);
        REQUIRE(move.ToSquare() == 1);
        REQUIRE(move.MovedPiece() == PAWN);
        REQUIRE(move.CapturedPiece() == ROOK);
        REQUIRE(move.PromotedPiece() == EMPTY_PIECE);
        REQUIRE(move.IsPromotion() == false);
        REQUIRE(move.IsDoublePush() == false);
        REQUIRE(move.IsEnPassant() == false);
        REQUIRE(move.IsCastle() == false);
        REQUIRE(move.IsCapture() == true);
        REQUIRE(move.IsPromotion() == false);
        REQUIRE(move.Uci() == "a1b1");
    }

    SECTION("Pawn Takes Bishop")
    {
        Move move = Move(0, 1, PAWN, BISHOP, EMPTY_PIECE, false, false, false);
        REQUIRE(move.FromSquare() == 0);
        REQUIRE(move.ToSquare() == 1);
        REQUIRE(move.MovedPiece() == PAWN);
        REQUIRE(move.CapturedPiece() == BISHOP);
        REQUIRE(move.PromotedPiece() == EMPTY_PIECE);
        REQUIRE(move.IsPromotion() == false);
        REQUIRE(move.IsDoublePush() == false);
        REQUIRE(move.IsEnPassant() == false);
        REQUIRE(move.IsCastle() == false);
        REQUIRE(move.IsCapture() == true);
        REQUIRE(move.Uci() == "a1b1");
    }

    SECTION("Pawn Takes Knight")
    {
        Move move = Move(0, 1, PAWN, KNIGHT, EMPTY_PIECE, false, false, false);
        REQUIRE(move.FromSquare() == 0);
        REQUIRE(move.ToSquare() == 1);
        REQUIRE(move.MovedPiece() == PAWN);
        REQUIRE(move.CapturedPiece() == KNIGHT);
        REQUIRE(move.PromotedPiece() == EMPTY_PIECE);
        REQUIRE(move.IsPromotion() == false);
        REQUIRE(move.IsDoublePush() == false);
        REQUIRE(move.IsEnPassant() == false);
        REQUIRE(move.IsCastle() == false);
        REQUIRE(move.IsCapture() == true);
        REQUIRE(move.Uci() == "a1b1");
    }

    SECTION("Pawn Takes Pawn")
    {
        Move move = Move(0, 1, PAWN, PAWN, EMPTY_PIECE, false, false, false);
        REQUIRE(move.FromSquare() == 0);
        REQUIRE(move.ToSquare() == 1);
        REQUIRE(move.MovedPiece() == PAWN);
        REQUIRE(move.CapturedPiece() == PAWN);
        REQUIRE(move.IsPromotion() == false);
        REQUIRE(move.IsDoublePush() == false);
        REQUIRE(move.IsEnPassant() == false);
        REQUIRE(move.IsCastle() == false);
        REQUIRE(move.IsCapture() == true);
        REQUIRE(move.Uci() == "a1b1");
    }
}