#include <catch2/catch_test_macros.hpp>

#include <codlib/move.hpp>

// TODO: Missing tests for get_uci()

TEST_CASE("Test Moves") {
    SECTION("Impossible Move") {
        Move move = Move(0, 63, PAWN, QUEEN, QUEEN, true, false, true);
        REQUIRE(move.get_from_square() == 0);
        REQUIRE(move.get_to_square() == 63);
        REQUIRE(move.get_piece_type() == PAWN);
        REQUIRE(move.get_captured_piece_type() == QUEEN);
        REQUIRE(move.get_promoted_piece_type() == QUEEN);
        REQUIRE(move.is_double_push() == true);
        REQUIRE(move.is_en_passant() == false);
        REQUIRE(move.is_castle() == true);
        REQUIRE(move.is_capture() == true);
        REQUIRE(move.get_uci() == "a1h8q");
    }

    SECTION("Pawn Takes King") {
        Move move = Move(0, 1, PAWN, KING, EMPTY_PIECE, false, false, false);
        REQUIRE(move.get_from_square() == 0);
        REQUIRE(move.get_to_square() == 1);
        REQUIRE(move.get_piece_type() == PAWN);
        REQUIRE(move.get_captured_piece_type() == KING);
        REQUIRE(move.get_promoted_piece_type() == EMPTY_PIECE);
        REQUIRE(move.is_promotion() == false);
        REQUIRE(move.is_double_push() == false);
        REQUIRE(move.is_en_passant() == false);
        REQUIRE(move.is_castle() == false);
        REQUIRE(move.is_capture() == true);
        REQUIRE(move.is_promotion() == false);
        REQUIRE(move.is_promotion() == false);
    }

    SECTION("Pawn Takes Queen") {
        Move move = Move(0, 1, PAWN, QUEEN, EMPTY_PIECE, false, false, false);
        REQUIRE(move.get_from_square() == 0);
        REQUIRE(move.get_to_square() == 1);
        REQUIRE(move.get_piece_type() == PAWN);
        REQUIRE(move.get_captured_piece_type() == QUEEN);
        REQUIRE(move.get_promoted_piece_type() == EMPTY_PIECE);
        REQUIRE(move.is_promotion() == false);
        REQUIRE(move.is_double_push() == false);
        REQUIRE(move.is_en_passant() == false);
        REQUIRE(move.is_castle() == false);
        REQUIRE(move.is_capture() == true);
        REQUIRE(move.get_uci() == "a1b1");
    }

    SECTION("Pawn Takes Rook") {
        Move move = Move(0, 1, PAWN, ROOK, EMPTY_PIECE, false, false, false);
        REQUIRE(move.get_from_square() == 0);
        REQUIRE(move.get_to_square() == 1);
        REQUIRE(move.get_piece_type() == PAWN);
        REQUIRE(move.get_captured_piece_type() == ROOK);
        REQUIRE(move.get_promoted_piece_type() == EMPTY_PIECE);
        REQUIRE(move.is_promotion() == false);
        REQUIRE(move.is_double_push() == false);
        REQUIRE(move.is_en_passant() == false);
        REQUIRE(move.is_castle() == false);
        REQUIRE(move.is_capture() == true);
        REQUIRE(move.is_promotion() == false);
    }

    SECTION("Pawn Takes Bishop") {
        Move move = Move(0, 1, PAWN, BISHOP, EMPTY_PIECE, false, false, false);
        REQUIRE(move.get_from_square() == 0);
        REQUIRE(move.get_to_square() == 1);
        REQUIRE(move.get_piece_type() == PAWN);
        REQUIRE(move.get_captured_piece_type() == BISHOP);
        REQUIRE(move.get_promoted_piece_type() == EMPTY_PIECE);
        REQUIRE(move.is_promotion() == false);
        REQUIRE(move.is_double_push() == false);
        REQUIRE(move.is_en_passant() == false);
        REQUIRE(move.is_castle() == false);
        REQUIRE(move.is_capture() == true);
        REQUIRE(move.get_uci() == "a1b1");
    }

    SECTION("Pawn Takes Knight") {
        Move move = Move(0, 1, PAWN, KNIGHT, EMPTY_PIECE, false, false, false);
        REQUIRE(move.get_from_square() == 0);
        REQUIRE(move.get_to_square() == 1);
        REQUIRE(move.get_piece_type() == PAWN);
        REQUIRE(move.get_captured_piece_type() == KNIGHT);
        REQUIRE(move.get_promoted_piece_type() == EMPTY_PIECE);
        REQUIRE(move.is_promotion() == false);
        REQUIRE(move.is_double_push() == false);
        REQUIRE(move.is_en_passant() == false);
        REQUIRE(move.is_castle() == false);
        REQUIRE(move.is_capture() == true);
        REQUIRE(move.get_uci() == "a1b1");
    }

    SECTION("Pawn Takes Pawn") {
        Move move = Move(0, 1, PAWN, PAWN, EMPTY_PIECE, false, false, false);
        REQUIRE(move.get_from_square() == 0);
        REQUIRE(move.get_to_square() == 1);
        REQUIRE(move.get_piece_type() == PAWN);
        REQUIRE(move.get_captured_piece_type() == PAWN);
        REQUIRE(move.is_promotion() == false);
        REQUIRE(move.is_double_push() == false);
        REQUIRE(move.is_en_passant() == false);
        REQUIRE(move.is_castle() == false);
        REQUIRE(move.is_capture() == true);
        REQUIRE(move.get_uci() == "a1b1");
    }
}