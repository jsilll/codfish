#include <catch2/catch_test_macros.hpp>

#include <codchess/codchess.hpp>

using namespace cod;

using chess::Move;
using chess::Piece;
using chess::Square;

TEST_CASE("Move Class", "[move]") {
    chess::Init();

    SECTION("Impossible Move") {
        const auto move =
            Move(Square::A1, Square::H8, Piece::PAWN, Piece::QUEEN,
                 Piece::QUEEN, true, false, true);
        REQUIRE(move.FromSquare() == Square::A1);
        REQUIRE(move.ToSquare() == Square::H8);
        REQUIRE(move.MovedPiece() == Piece::PAWN);
        REQUIRE(move.CapturedPiece() == Piece::QUEEN);
        REQUIRE(move.PromotedPiece() == Piece::QUEEN);
        REQUIRE(move.IsDoublePush() == true);
        REQUIRE(move.IsEnPassant() == false);
        REQUIRE(move.IsCastle() == true);
        REQUIRE(move.IsCapture() == true);
        REQUIRE(move.ToString() == "a1h8q");
    }

    SECTION("Pawn Takes King") {
        const auto move =
            Move(Square::A1, Square::B1, Piece::PAWN, Piece::KING,
                 Piece::EMPTY_PIECE, false, false, false);
        REQUIRE(move.FromSquare() == Square::A1);
        REQUIRE(move.ToSquare() == Square::B1);
        REQUIRE(move.MovedPiece() == Piece::PAWN);
        REQUIRE(move.CapturedPiece() == Piece::KING);
        REQUIRE(move.PromotedPiece() == Piece::EMPTY_PIECE);
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
            Move(Square::A1, Square::B1, Piece::PAWN, Piece::QUEEN,
                 Piece::EMPTY_PIECE, false, false, false);
        REQUIRE(move.FromSquare() == Square::A1);
        REQUIRE(move.ToSquare() == Square::B1);
        REQUIRE(move.MovedPiece() == Piece::PAWN);
        REQUIRE(move.CapturedPiece() == Piece::QUEEN);
        REQUIRE(move.PromotedPiece() == Piece::EMPTY_PIECE);
        REQUIRE(move.IsPromotion() == false);
        REQUIRE(move.IsDoublePush() == false);
        REQUIRE(move.IsEnPassant() == false);
        REQUIRE(move.IsCastle() == false);
        REQUIRE(move.IsCapture() == true);
        REQUIRE(move.ToString() == "a1b1");
    }

    SECTION("Pawn Takes Rook") {
        const auto move =
            Move(Square::A1, Square::B1, Piece::PAWN, Piece::ROOK,
                 Piece::EMPTY_PIECE, false, false, false);
        REQUIRE(move.FromSquare() == Square::A1);
        REQUIRE(move.ToSquare() == Square::B1);
        REQUIRE(move.MovedPiece() == Piece::PAWN);
        REQUIRE(move.CapturedPiece() == Piece::ROOK);
        REQUIRE(move.PromotedPiece() == Piece::EMPTY_PIECE);
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
            Move(Square::A1, Square::B1, Piece::PAWN, Piece::BISHOP,
                 Piece::EMPTY_PIECE, false, false, false);
        REQUIRE(move.FromSquare() == Square::A1);
        REQUIRE(move.ToSquare() == Square::B1);
        REQUIRE(move.MovedPiece() == Piece::PAWN);
        REQUIRE(move.CapturedPiece() == Piece::BISHOP);
        REQUIRE(move.PromotedPiece() == Piece::EMPTY_PIECE);
        REQUIRE(move.IsPromotion() == false);
        REQUIRE(move.IsDoublePush() == false);
        REQUIRE(move.IsEnPassant() == false);
        REQUIRE(move.IsCastle() == false);
        REQUIRE(move.IsCapture() == true);
        REQUIRE(move.ToString() == "a1b1");
    }

    SECTION("Pawn Takes Knight") {
        const auto move =
            Move(Square::A1, Square::B1, Piece::PAWN, Piece::KNIGHT,
                 Piece::EMPTY_PIECE, false, false, false);
        REQUIRE(move.FromSquare() == Square::A1);
        REQUIRE(move.ToSquare() == Square::B1);
        REQUIRE(move.MovedPiece() == Piece::PAWN);
        REQUIRE(move.CapturedPiece() == Piece::KNIGHT);
        REQUIRE(move.PromotedPiece() == Piece::EMPTY_PIECE);
        REQUIRE(move.IsPromotion() == false);
        REQUIRE(move.IsDoublePush() == false);
        REQUIRE(move.IsEnPassant() == false);
        REQUIRE(move.IsCastle() == false);
        REQUIRE(move.IsCapture() == true);
        REQUIRE(move.ToString() == "a1b1");
    }

    SECTION("Pawn Takes Pawn") {
        const auto move =
            Move(Square::A1, Square::B1, Piece::PAWN, Piece::PAWN,
                 Piece::EMPTY_PIECE, false, false, false);
        REQUIRE(move.FromSquare() == Square::A1);
        REQUIRE(move.ToSquare() == Square::B1);
        REQUIRE(move.MovedPiece() == Piece::PAWN);
        REQUIRE(move.CapturedPiece() == Piece::PAWN);
        REQUIRE(move.IsPromotion() == false);
        REQUIRE(move.IsDoublePush() == false);
        REQUIRE(move.IsEnPassant() == false);
        REQUIRE(move.IsCastle() == false);
        REQUIRE(move.IsCapture() == true);
        REQUIRE(move.ToString() == "a1b1");
    }
}