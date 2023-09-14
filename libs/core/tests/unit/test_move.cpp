#include <catch2/catch_test_macros.hpp>

#include <core/codchess.hpp>

using namespace codfish;

using core::Move;
using core::Piece;
using core::Square;

TEST_CASE("Move Class", "[move]") {
    core::Init();

    SECTION("Impossible Move") {
        const auto move =
            Move(Square::A1, Square::H8, Piece::Pawn, Piece::Queen,
                 Piece::Queen, true, false, true);
        REQUIRE(move.FromSquare() == Square::A1);
        REQUIRE(move.ToSquare() == Square::H8);
        REQUIRE(move.MovedPiece() == Piece::Pawn);
        REQUIRE(move.CapturedPiece() == Piece::Queen);
        REQUIRE(move.PromotedPiece() == Piece::Queen);
        REQUIRE(move.IsDoublePush() == true);
        REQUIRE(move.IsEnPassant() == false);
        REQUIRE(move.IsCastle() == true);
        REQUIRE(move.IsCapture() == true);
        REQUIRE(move.ToString() == "a1h8q");
    }

    SECTION("Pawn Takes King") {
        const auto move =
            Move(Square::A1, Square::B1, Piece::Pawn, Piece::King,
                 Piece::Empty, false, false, false);
        REQUIRE(move.FromSquare() == Square::A1);
        REQUIRE(move.ToSquare() == Square::B1);
        REQUIRE(move.MovedPiece() == Piece::Pawn);
        REQUIRE(move.CapturedPiece() == Piece::King);
        REQUIRE(move.PromotedPiece() == Piece::Empty);
        REQUIRE(move.IsPromotion() == false);
        REQUIRE(move.IsDoublePush() == false);
        REQUIRE(move.IsEnPassant() == false);
        REQUIRE(move.IsCastle() == false);
        REQUIRE(move.IsCapture() == true);
        REQUIRE(move.IsPromotion() == false);
        REQUIRE(move.IsPromotion() == false);
        REQUIRE(move.ToString() == "a1b1");
    }

    SECTION("Pawn Takes Queen") {
        const auto move =
            Move(Square::A1, Square::B1, Piece::Pawn, Piece::Queen,
                 Piece::Empty, false, false, false);
        REQUIRE(move.FromSquare() == Square::A1);
        REQUIRE(move.ToSquare() == Square::B1);
        REQUIRE(move.MovedPiece() == Piece::Pawn);
        REQUIRE(move.CapturedPiece() == Piece::Queen);
        REQUIRE(move.PromotedPiece() == Piece::Empty);
        REQUIRE(move.IsPromotion() == false);
        REQUIRE(move.IsDoublePush() == false);
        REQUIRE(move.IsEnPassant() == false);
        REQUIRE(move.IsCastle() == false);
        REQUIRE(move.IsCapture() == true);
        REQUIRE(move.ToString() == "a1b1");
    }

    SECTION("Pawn Takes Rook") {
        const auto move =
            Move(Square::A1, Square::B1, Piece::Pawn, Piece::Rook,
                 Piece::Empty, false, false, false);
        REQUIRE(move.FromSquare() == Square::A1);
        REQUIRE(move.ToSquare() == Square::B1);
        REQUIRE(move.MovedPiece() == Piece::Pawn);
        REQUIRE(move.CapturedPiece() == Piece::Rook);
        REQUIRE(move.PromotedPiece() == Piece::Empty);
        REQUIRE(move.IsPromotion() == false);
        REQUIRE(move.IsDoublePush() == false);
        REQUIRE(move.IsEnPassant() == false);
        REQUIRE(move.IsCastle() == false);
        REQUIRE(move.IsCapture() == true);
        REQUIRE(move.IsPromotion() == false);
        REQUIRE(move.ToString() == "a1b1");
    }

    SECTION("Pawn Takes Bishop") {
        const auto move =
            Move(Square::A1, Square::B1, Piece::Pawn, Piece::Bishop,
                 Piece::Empty, false, false, false);
        REQUIRE(move.FromSquare() == Square::A1);
        REQUIRE(move.ToSquare() == Square::B1);
        REQUIRE(move.MovedPiece() == Piece::Pawn);
        REQUIRE(move.CapturedPiece() == Piece::Bishop);
        REQUIRE(move.PromotedPiece() == Piece::Empty);
        REQUIRE(move.IsPromotion() == false);
        REQUIRE(move.IsDoublePush() == false);
        REQUIRE(move.IsEnPassant() == false);
        REQUIRE(move.IsCastle() == false);
        REQUIRE(move.IsCapture() == true);
        REQUIRE(move.ToString() == "a1b1");
    }

    SECTION("Pawn Takes Knight") {
        const auto move =
            Move(Square::A1, Square::B1, Piece::Pawn, Piece::Knight,
                 Piece::Empty, false, false, false);
        REQUIRE(move.FromSquare() == Square::A1);
        REQUIRE(move.ToSquare() == Square::B1);
        REQUIRE(move.MovedPiece() == Piece::Pawn);
        REQUIRE(move.CapturedPiece() == Piece::Knight);
        REQUIRE(move.PromotedPiece() == Piece::Empty);
        REQUIRE(move.IsPromotion() == false);
        REQUIRE(move.IsDoublePush() == false);
        REQUIRE(move.IsEnPassant() == false);
        REQUIRE(move.IsCastle() == false);
        REQUIRE(move.IsCapture() == true);
        REQUIRE(move.ToString() == "a1b1");
    }

    SECTION("Pawn Takes Pawn") {
        const auto move =
            Move(Square::A1, Square::B1, Piece::Pawn, Piece::Pawn,
                 Piece::Empty, false, false, false);
        REQUIRE(move.FromSquare() == Square::A1);
        REQUIRE(move.ToSquare() == Square::B1);
        REQUIRE(move.MovedPiece() == Piece::Pawn);
        REQUIRE(move.CapturedPiece() == Piece::Pawn);
        REQUIRE(move.IsPromotion() == false);
        REQUIRE(move.IsDoublePush() == false);
        REQUIRE(move.IsEnPassant() == false);
        REQUIRE(move.IsCastle() == false);
        REQUIRE(move.IsCapture() == true);
        REQUIRE(move.ToString() == "a1b1");
    }
}