#include <catch2/catch_test_macros.hpp>

#include <codchess/codchess.hpp>

using namespace cod;

using chess::Move;
using chess::PieceType;
using chess::Square;

TEST_CASE("Move Class", "[move]") {
    chess::Init();

    SECTION("Impossible Move") {
        const auto move =
            Move(Square::A1, Square::H8, PieceType::PAWN, PieceType::QUEEN,
                 PieceType::QUEEN, true, false, true);
        REQUIRE(move.FromSquare() == Square::A1);
        REQUIRE(move.ToSquare() == Square::H8);
        REQUIRE(move.MovedPiece() == PieceType::PAWN);
        REQUIRE(move.CapturedPiece() == PieceType::QUEEN);
        REQUIRE(move.PromotedPiece() == PieceType::QUEEN);
        REQUIRE(move.IsDoublePush() == true);
        REQUIRE(move.IsEnPassant() == false);
        REQUIRE(move.IsCastle() == true);
        REQUIRE(move.IsCapture() == true);
        REQUIRE(move.ToString() == "a1h8q");
    }

    SECTION("Pawn Takes King") {
        const auto move =
            Move(Square::A1, Square::B1, PieceType::PAWN, PieceType::KING,
                 PieceType::EMPTY_PIECE, false, false, false);
        REQUIRE(move.FromSquare() == Square::A1);
        REQUIRE(move.ToSquare() == Square::B1);
        REQUIRE(move.MovedPiece() == PieceType::PAWN);
        REQUIRE(move.CapturedPiece() == PieceType::KING);
        REQUIRE(move.PromotedPiece() == PieceType::EMPTY_PIECE);
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
            Move(Square::A1, Square::B1, PieceType::PAWN, PieceType::QUEEN,
                 PieceType::EMPTY_PIECE, false, false, false);
        REQUIRE(move.FromSquare() == Square::A1);
        REQUIRE(move.ToSquare() == Square::B1);
        REQUIRE(move.MovedPiece() == PieceType::PAWN);
        REQUIRE(move.CapturedPiece() == PieceType::QUEEN);
        REQUIRE(move.PromotedPiece() == PieceType::EMPTY_PIECE);
        REQUIRE(move.IsPromotion() == false);
        REQUIRE(move.IsDoublePush() == false);
        REQUIRE(move.IsEnPassant() == false);
        REQUIRE(move.IsCastle() == false);
        REQUIRE(move.IsCapture() == true);
        REQUIRE(move.ToString() == "a1b1");
    }

    SECTION("Pawn Takes Rook") {
        const auto move =
            Move(Square::A1, Square::B1, PieceType::PAWN, PieceType::ROOK,
                 PieceType::EMPTY_PIECE, false, false, false);
        REQUIRE(move.FromSquare() == Square::A1);
        REQUIRE(move.ToSquare() == Square::B1);
        REQUIRE(move.MovedPiece() == PieceType::PAWN);
        REQUIRE(move.CapturedPiece() == PieceType::ROOK);
        REQUIRE(move.PromotedPiece() == PieceType::EMPTY_PIECE);
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
            Move(Square::A1, Square::B1, PieceType::PAWN, PieceType::BISHOP,
                 PieceType::EMPTY_PIECE, false, false, false);
        REQUIRE(move.FromSquare() == Square::A1);
        REQUIRE(move.ToSquare() == Square::B1);
        REQUIRE(move.MovedPiece() == PieceType::PAWN);
        REQUIRE(move.CapturedPiece() == PieceType::BISHOP);
        REQUIRE(move.PromotedPiece() == PieceType::EMPTY_PIECE);
        REQUIRE(move.IsPromotion() == false);
        REQUIRE(move.IsDoublePush() == false);
        REQUIRE(move.IsEnPassant() == false);
        REQUIRE(move.IsCastle() == false);
        REQUIRE(move.IsCapture() == true);
        REQUIRE(move.ToString() == "a1b1");
    }

    SECTION("Pawn Takes Knight") {
        const auto move =
            Move(Square::A1, Square::B1, PieceType::PAWN, PieceType::KNIGHT,
                 PieceType::EMPTY_PIECE, false, false, false);
        REQUIRE(move.FromSquare() == Square::A1);
        REQUIRE(move.ToSquare() == Square::B1);
        REQUIRE(move.MovedPiece() == PieceType::PAWN);
        REQUIRE(move.CapturedPiece() == PieceType::KNIGHT);
        REQUIRE(move.PromotedPiece() == PieceType::EMPTY_PIECE);
        REQUIRE(move.IsPromotion() == false);
        REQUIRE(move.IsDoublePush() == false);
        REQUIRE(move.IsEnPassant() == false);
        REQUIRE(move.IsCastle() == false);
        REQUIRE(move.IsCapture() == true);
        REQUIRE(move.ToString() == "a1b1");
    }

    SECTION("Pawn Takes Pawn") {
        const auto move =
            Move(Square::A1, Square::B1, PieceType::PAWN, PieceType::PAWN,
                 PieceType::EMPTY_PIECE, false, false, false);
        REQUIRE(move.FromSquare() == Square::A1);
        REQUIRE(move.ToSquare() == Square::B1);
        REQUIRE(move.MovedPiece() == PieceType::PAWN);
        REQUIRE(move.CapturedPiece() == PieceType::PAWN);
        REQUIRE(move.IsPromotion() == false);
        REQUIRE(move.IsDoublePush() == false);
        REQUIRE(move.IsEnPassant() == false);
        REQUIRE(move.IsCastle() == false);
        REQUIRE(move.IsCapture() == true);
        REQUIRE(move.ToString() == "a1b1");
    }
}