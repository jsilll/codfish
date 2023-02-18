#include <catch2/catch_test_macros.hpp>

#include <codlib/board.hpp>

#include <iostream>

TEST_CASE("Board::active() returns the correct color", "[board]")
{
    SECTION("Position 1")
    {
        const auto board = Board();

        REQUIRE(board.active() == Color::WHITE);
    }

    SECTION("Position 2")
    {
        const auto board = Board("8/8/8/8/8/8/8/8 w - - 0 1");

        REQUIRE(board.active() == Color::WHITE);
    }

    SECTION("Position 3")
    {
        const auto board = Board("8/8/8/8/8/8/8/8 b - - 0 1");

        REQUIRE(board.active() == Color::BLACK);
    }
}

TEST_CASE("Board::inactive() returns the correct color", "[board]")
{
    SECTION("Position 1")
    {
        const auto board = Board();

        REQUIRE(board.inactive() == Color::BLACK);
    }

    SECTION("Position 2")
    {
        const auto board = Board("8/8/8/8/8/8/8/8 w - - 0 1");

        REQUIRE(board.inactive() == Color::BLACK);
    }

    SECTION("Position 3")
    {
        const auto board = Board("8/8/8/8/8/8/8/8 b - - 0 1");

        REQUIRE(board.inactive() == Color::WHITE);
    }
}

TEST_CASE("Board::castling_availability() returns the correct castling_availability availability", "[board]") {
    SECTION("Position 1") {
        const auto board = Board();

        REQUIRE(board.castling_availability() == CastlingAvailability::ALL);
    }

    SECTION("Position 2") {
        const auto board = Board("8/8/8/8/8/8/8/8 w - - 0 1");

        REQUIRE(board.castling_availability() == CastlingAvailability::NONE);
    }

    SECTION("Position 3") {
        const auto board = Board("8/8/8/8/8/8/8/8 w KQkq - 0 1");

        REQUIRE(board.castling_availability() == CastlingAvailability::ALL);
    }

    SECTION("Position 4") {
        const auto board = Board("8/8/8/8/8/8/8/8 w KQk - 0 1");

        REQUIRE(board.castling_availability() == (CastlingAvailability::WHITE_KING | CastlingAvailability::WHITE_QUEEN |
                                                  CastlingAvailability::BLACK_KING));
    }

    SECTION("Position 5") {
        const auto board = Board("8/8/8/8/8/8/8/8 w KQq - 0 1");

        REQUIRE(board.castling_availability() == (CastlingAvailability::WHITE_KING | CastlingAvailability::WHITE_QUEEN |
                                                  CastlingAvailability::BLACK_QUEEN));
    }

    SECTION("Position 6") {
        const auto board = Board("8/8/8/8/8/8/8/8 w Kkq - 0 1");

        REQUIRE(board.castling_availability() == (CastlingAvailability::WHITE_KING | CastlingAvailability::BLACK_KING |
                                                  CastlingAvailability::BLACK_QUEEN));
    }

    SECTION("Position 7") {
        const auto board = Board("8/8/8/8/8/8/8/8 w Qkq - 0 1");

        REQUIRE(board.castling_availability() == (CastlingAvailability::WHITE_QUEEN | CastlingAvailability::BLACK_KING |
                                                  CastlingAvailability::BLACK_QUEEN));
    }

    SECTION("Position 8") {
        const auto board = Board("8/8/8/8/8/8/8/8 w Kk - 0 1");

        REQUIRE(board.castling_availability() == (CastlingAvailability::WHITE_KING | CastlingAvailability::BLACK_KING));
    }

    SECTION("Position 9") {
        const auto board = Board("8/8/8/8/8/8/8/8 w Kq - 0 1");

        REQUIRE(board.castling_availability() ==
                (CastlingAvailability::WHITE_KING | CastlingAvailability::BLACK_QUEEN));
    }

    SECTION("Position 10") {
        const auto board = Board("8/8/8/8/8/8/8/8 w Qk - 0 1");

        REQUIRE(board.castling_availability() ==
                (CastlingAvailability::WHITE_QUEEN | CastlingAvailability::BLACK_KING));
    }

    SECTION("Position 11") {
        const auto board = Board("8/8/8/8/8/8/8/8 w Qq - 0 1");

        REQUIRE(board.castling_availability() ==
                (CastlingAvailability::WHITE_QUEEN | CastlingAvailability::BLACK_QUEEN));
    }

    SECTION("Position 12") {
        const auto board = Board("8/8/8/8/8/8/8/8 w K - 0 1");

        REQUIRE(board.castling_availability() == (CastlingAvailability::WHITE_KING));
    }

    SECTION("Position 13") {
        const auto board = Board("8/8/8/8/8/8/8/8 w Q - 0 1");

        REQUIRE(board.castling_availability() == (CastlingAvailability::WHITE_QUEEN));
    }

    SECTION("Position 14") {
        const auto board = Board("8/8/8/8/8/8/8/8 w k - 0 1");

        REQUIRE(board.castling_availability() == (CastlingAvailability::BLACK_KING));
    }

    SECTION("Position 15") {
        const auto board = Board("8/8/8/8/8/8/8/8 w q- 0 1");

        REQUIRE(board.castling_availability() == (CastlingAvailability::BLACK_QUEEN));
    }
}

TEST_CASE("Board::half_move_clock() returns the correct half move clock", "[board]") {
    SECTION("Position 1") {
        const auto board = Board();

        REQUIRE(board.half_move_clock() == 0);
    }

    SECTION("Position 2") {
        const auto board = Board("8/8/8/8/8/8/8/8 w - - 0 1");

        REQUIRE(board.half_move_clock() == 0);
    }

    SECTION("Position 3") {
        const auto board = Board("8/8/8/8/8/8/8/8 w - - 1 1");

        REQUIRE(board.half_move_clock() == 1);
    }

    SECTION("Position 4") {
        const auto board = Board("8/8/8/8/8/8/8/8 w - - 2 1");

        REQUIRE(board.half_move_clock() == 2);
    }

    SECTION("Position 5") {
        const auto board = Board("8/8/8/8/8/8/8/8 w - - 3 1");

        REQUIRE(board.half_move_clock() == 3);
    }
}

TEST_CASE("Board::en_passant_square() returns the correct en passant square", "[board]") {
    SECTION("Position 1") {
        const auto board = Board();

        REQUIRE(board.en_passant_square() == Square::EMPTY_SQUARE);
    }

    SECTION("Position 2") {
        const auto board = Board("8/8/8/8/8/8/8/8 w - - 0 1");

        REQUIRE(board.en_passant_square() == Square::EMPTY_SQUARE);
    }

    SECTION("Position 3") {
        const auto board = Board("8/8/8/8/8/8/8/8 w - a3 0 1");

        REQUIRE(board.en_passant_square() == Square::A3);
    }

    SECTION("Position 4") {
        const auto board = Board("8/8/8/8/8/8/8/8 w - b3 0 1");

        REQUIRE(board.en_passant_square() == Square::B3);
    }

    SECTION("Position 5") {
        const auto board = Board("8/8/8/8/8/8/8/8 w - c3 0 1");

        REQUIRE(board.en_passant_square() == Square::C3);
    }
}

TEST_CASE("Board::piece() returns the correct piece", "[board]") {
    SECTION("Position 1") {
        const auto board = Board();

        REQUIRE(board.piece(Square::A1) == Board::Piece{Color::WHITE, PieceType::ROOK});
        REQUIRE(board.piece(Square::B1) == Board::Piece{Color::WHITE, PieceType::KNIGHT});
        REQUIRE(board.piece(Square::C1) == Board::Piece{Color::WHITE, PieceType::BISHOP});
        REQUIRE(board.piece(Square::D1) == Board::Piece{Color::WHITE, PieceType::QUEEN});
        REQUIRE(board.piece(Square::E1) == Board::Piece{Color::WHITE, PieceType::KING});
        REQUIRE(board.piece(Square::F1) == Board::Piece{Color::WHITE, PieceType::BISHOP});
        REQUIRE(board.piece(Square::G1) == Board::Piece{Color::WHITE, PieceType::KNIGHT});
        REQUIRE(board.piece(Square::H1) == Board::Piece{Color::WHITE, PieceType::ROOK});

        REQUIRE(board.piece(Square::A2) == Board::Piece{Color::WHITE, PieceType::PAWN});
        REQUIRE(board.piece(Square::B2) == Board::Piece{Color::WHITE, PieceType::PAWN});
        REQUIRE(board.piece(Square::C2) == Board::Piece{Color::WHITE, PieceType::PAWN});
        REQUIRE(board.piece(Square::D2) == Board::Piece{Color::WHITE, PieceType::PAWN});
        REQUIRE(board.piece(Square::E2) == Board::Piece{Color::WHITE, PieceType::PAWN});
        REQUIRE(board.piece(Square::F2) == Board::Piece{Color::WHITE, PieceType::PAWN});
        REQUIRE(board.piece(Square::G2) == Board::Piece{Color::WHITE, PieceType::PAWN});
        REQUIRE(board.piece(Square::H2) == Board::Piece{Color::WHITE, PieceType::PAWN});

        REQUIRE(board.piece(Square::A3) == Board::Piece{Color::BOTH, PieceType::EMPTY_PIECE});
        REQUIRE(board.piece(Square::B3) == Board::Piece{Color::BOTH, PieceType::EMPTY_PIECE});
        REQUIRE(board.piece(Square::C3) == Board::Piece{Color::BOTH, PieceType::EMPTY_PIECE});
        REQUIRE(board.piece(Square::D3) == Board::Piece{Color::BOTH, PieceType::EMPTY_PIECE});
        REQUIRE(board.piece(Square::E3) == Board::Piece{Color::BOTH, PieceType::EMPTY_PIECE});
        REQUIRE(board.piece(Square::F3) == Board::Piece{Color::BOTH, PieceType::EMPTY_PIECE});
        REQUIRE(board.piece(Square::G3) == Board::Piece{Color::BOTH, PieceType::EMPTY_PIECE});
        REQUIRE(board.piece(Square::H3) == Board::Piece{Color::BOTH, PieceType::EMPTY_PIECE});

        REQUIRE(board.piece(Square::A4) == Board::Piece{Color::BOTH, PieceType::EMPTY_PIECE});
        REQUIRE(board.piece(Square::B4) == Board::Piece{Color::BOTH, PieceType::EMPTY_PIECE});
        REQUIRE(board.piece(Square::C4) == Board::Piece{Color::BOTH, PieceType::EMPTY_PIECE});
        REQUIRE(board.piece(Square::D4) == Board::Piece{Color::BOTH, PieceType::EMPTY_PIECE});
        REQUIRE(board.piece(Square::E4) == Board::Piece{Color::BOTH, PieceType::EMPTY_PIECE});
        REQUIRE(board.piece(Square::F4) == Board::Piece{Color::BOTH, PieceType::EMPTY_PIECE});
        REQUIRE(board.piece(Square::G4) == Board::Piece{Color::BOTH, PieceType::EMPTY_PIECE});
        REQUIRE(board.piece(Square::H4) == Board::Piece{Color::BOTH, PieceType::EMPTY_PIECE});

        REQUIRE(board.piece(Square::A5) == Board::Piece{Color::BOTH, PieceType::EMPTY_PIECE});
        REQUIRE(board.piece(Square::B5) == Board::Piece{Color::BOTH, PieceType::EMPTY_PIECE});
        REQUIRE(board.piece(Square::C5) == Board::Piece{Color::BOTH, PieceType::EMPTY_PIECE});
        REQUIRE(board.piece(Square::D5) == Board::Piece{Color::BOTH, PieceType::EMPTY_PIECE});
        REQUIRE(board.piece(Square::E5) == Board::Piece{Color::BOTH, PieceType::EMPTY_PIECE});
        REQUIRE(board.piece(Square::F5) == Board::Piece{Color::BOTH, PieceType::EMPTY_PIECE});
        REQUIRE(board.piece(Square::G5) == Board::Piece{Color::BOTH, PieceType::EMPTY_PIECE});
        REQUIRE(board.piece(Square::H5) == Board::Piece{Color::BOTH, PieceType::EMPTY_PIECE});

        REQUIRE(board.piece(Square::A6) == Board::Piece{Color::BOTH, PieceType::EMPTY_PIECE});
        REQUIRE(board.piece(Square::B6) == Board::Piece{Color::BOTH, PieceType::EMPTY_PIECE});
        REQUIRE(board.piece(Square::C6) == Board::Piece{Color::BOTH, PieceType::EMPTY_PIECE});
        REQUIRE(board.piece(Square::D6) == Board::Piece{Color::BOTH, PieceType::EMPTY_PIECE});
        REQUIRE(board.piece(Square::E6) == Board::Piece{Color::BOTH, PieceType::EMPTY_PIECE});
        REQUIRE(board.piece(Square::F6) == Board::Piece{Color::BOTH, PieceType::EMPTY_PIECE});
        REQUIRE(board.piece(Square::G6) == Board::Piece{Color::BOTH, PieceType::EMPTY_PIECE});
        REQUIRE(board.piece(Square::H6) == Board::Piece{Color::BOTH, PieceType::EMPTY_PIECE});

        REQUIRE(board.piece(Square::A7) == Board::Piece{Color::BLACK, PieceType::PAWN});
        REQUIRE(board.piece(Square::B7) == Board::Piece{Color::BLACK, PieceType::PAWN});
        REQUIRE(board.piece(Square::C7) == Board::Piece{Color::BLACK, PieceType::PAWN});
        REQUIRE(board.piece(Square::D7) == Board::Piece{Color::BLACK, PieceType::PAWN});
        REQUIRE(board.piece(Square::E7) == Board::Piece{Color::BLACK, PieceType::PAWN});
        REQUIRE(board.piece(Square::F7) == Board::Piece{Color::BLACK, PieceType::PAWN});
        REQUIRE(board.piece(Square::G7) == Board::Piece{Color::BLACK, PieceType::PAWN});
        REQUIRE(board.piece(Square::H7) == Board::Piece{Color::BLACK, PieceType::PAWN});

        REQUIRE(board.piece(Square::A8) == Board::Piece{Color::BLACK, PieceType::ROOK});
        REQUIRE(board.piece(Square::B8) == Board::Piece{Color::BLACK, PieceType::KNIGHT});
        REQUIRE(board.piece(Square::C8) == Board::Piece{Color::BLACK, PieceType::BISHOP});
        REQUIRE(board.piece(Square::D8) == Board::Piece{Color::BLACK, PieceType::QUEEN});
        REQUIRE(board.piece(Square::E8) == Board::Piece{Color::BLACK, PieceType::KING});
        REQUIRE(board.piece(Square::F8) == Board::Piece{Color::BLACK, PieceType::BISHOP});
        REQUIRE(board.piece(Square::G8) == Board::Piece{Color::BLACK, PieceType::KNIGHT});
        REQUIRE(board.piece(Square::H8) == Board::Piece{Color::BLACK, PieceType::ROOK});
    }
}

TEST_CASE("Board::pieces() returns the correct pieces", "[board]") {
    SECTION("Position 1") {
        const auto board = Board();

        REQUIRE(board.pieces(Color::WHITE, PieceType::PAWN) == 0x000000000000FF00);
    }
}

TEST_CASE("Board::full_move_number() returns the correct full move number", "[board]") {
    SECTION("Position 1") {
        const auto board = Board();

        REQUIRE(board.full_move_number() == 1);
    }

    SECTION("Position 2") {
        const auto board = Board("8/8/8/8/8/8/8/8 w - - 0 1");

        REQUIRE(board.full_move_number() == 1);
    }

    SECTION("Position 3") {
        const auto board = Board("8/8/8/8/8/8/8/8 w - - 0 2");

        REQUIRE(board.full_move_number() == 2);
    }

    SECTION("Position 4") {
        const auto board = Board("8/8/8/8/8/8/8/8 w - - 0 3");

        REQUIRE(board.full_move_number() == 3);
    }
}

TEST_CASE("Board::Board() sets the board correctly", "[board]")
{
    SECTION("Position 1")
    {
        const auto board = Board();

        REQUIRE(board.half_move_clock() == 0);
        REQUIRE(board.full_move_number() == 1);
        REQUIRE(board.active() == Color::WHITE);
        REQUIRE(board.en_passant_square() == Square::EMPTY_SQUARE);
        REQUIRE(board.castling_availability() == CastlingAvailability::ALL);

        REQUIRE(board.piece(Square::A1) == Board::Piece{Color::WHITE, PieceType::ROOK});
        REQUIRE(board.piece(Square::A2) == Board::Piece{Color::WHITE, PieceType::PAWN});
        REQUIRE(board.piece(Square::A3) == Board::Piece{Color::BOTH, PieceType::EMPTY_PIECE});
        REQUIRE(board.piece(Square::A4) == Board::Piece{Color::BOTH, PieceType::EMPTY_PIECE});
        REQUIRE(board.piece(Square::A5) == Board::Piece{Color::BOTH, PieceType::EMPTY_PIECE});
        REQUIRE(board.piece(Square::A6) == Board::Piece{Color::BOTH, PieceType::EMPTY_PIECE});
        REQUIRE(board.piece(Square::A7) == Board::Piece{Color::BLACK, PieceType::PAWN});
        REQUIRE(board.piece(Square::A8) == Board::Piece{Color::BLACK, PieceType::ROOK});

        REQUIRE(board.piece(Square::B1) == Board::Piece{Color::WHITE, PieceType::KNIGHT});
        REQUIRE(board.piece(Square::B2) == Board::Piece{Color::WHITE, PieceType::PAWN});
        REQUIRE(board.piece(Square::B3) == Board::Piece{Color::BOTH, PieceType::EMPTY_PIECE});
        REQUIRE(board.piece(Square::B4) == Board::Piece{Color::BOTH, PieceType::EMPTY_PIECE});
        REQUIRE(board.piece(Square::B5) == Board::Piece{Color::BOTH, PieceType::EMPTY_PIECE});
        REQUIRE(board.piece(Square::B6) == Board::Piece{Color::BOTH, PieceType::EMPTY_PIECE});
        REQUIRE(board.piece(Square::B7) == Board::Piece{Color::BLACK, PieceType::PAWN});
        REQUIRE(board.piece(Square::B8) == Board::Piece{Color::BLACK, PieceType::KNIGHT});

        REQUIRE(board.piece(Square::C1) == Board::Piece{Color::WHITE, PieceType::BISHOP});
        REQUIRE(board.piece(Square::C2) == Board::Piece{Color::WHITE, PieceType::PAWN});
        REQUIRE(board.piece(Square::C3) == Board::Piece{Color::BOTH, PieceType::EMPTY_PIECE});
        REQUIRE(board.piece(Square::C4) == Board::Piece{Color::BOTH, PieceType::EMPTY_PIECE});
        REQUIRE(board.piece(Square::C5) == Board::Piece{Color::BOTH, PieceType::EMPTY_PIECE});
        REQUIRE(board.piece(Square::C6) == Board::Piece{Color::BOTH, PieceType::EMPTY_PIECE});
        REQUIRE(board.piece(Square::C7) == Board::Piece{Color::BLACK, PieceType::PAWN});
        REQUIRE(board.piece(Square::C8) == Board::Piece{Color::BLACK, PieceType::BISHOP});

        REQUIRE(board.piece(Square::D1) == Board::Piece{Color::WHITE, PieceType::QUEEN});
        REQUIRE(board.piece(Square::D2) == Board::Piece{Color::WHITE, PieceType::PAWN});
        REQUIRE(board.piece(Square::D3) == Board::Piece{Color::BOTH, PieceType::EMPTY_PIECE});
        REQUIRE(board.piece(Square::D4) == Board::Piece{Color::BOTH, PieceType::EMPTY_PIECE});
        REQUIRE(board.piece(Square::D5) == Board::Piece{Color::BOTH, PieceType::EMPTY_PIECE});
        REQUIRE(board.piece(Square::D6) == Board::Piece{Color::BOTH, PieceType::EMPTY_PIECE});
        REQUIRE(board.piece(Square::D7) == Board::Piece{Color::BLACK, PieceType::PAWN});
        REQUIRE(board.piece(Square::D8) == Board::Piece{Color::BLACK, PieceType::QUEEN});

        REQUIRE(board.piece(Square::E1) == Board::Piece{Color::WHITE, PieceType::KING});
        REQUIRE(board.piece(Square::E2) == Board::Piece{Color::WHITE, PieceType::PAWN});
        REQUIRE(board.piece(Square::E3) == Board::Piece{Color::BOTH, PieceType::EMPTY_PIECE});
        REQUIRE(board.piece(Square::E4) == Board::Piece{Color::BOTH, PieceType::EMPTY_PIECE});
        REQUIRE(board.piece(Square::E5) == Board::Piece{Color::BOTH, PieceType::EMPTY_PIECE});
        REQUIRE(board.piece(Square::E6) == Board::Piece{Color::BOTH, PieceType::EMPTY_PIECE});
        REQUIRE(board.piece(Square::E7) == Board::Piece{Color::BLACK, PieceType::PAWN});
        REQUIRE(board.piece(Square::E8) == Board::Piece{Color::BLACK, PieceType::KING});

        REQUIRE(board.piece(Square::F1) == Board::Piece{Color::WHITE, PieceType::BISHOP});
        REQUIRE(board.piece(Square::F2) == Board::Piece{Color::WHITE, PieceType::PAWN});
        REQUIRE(board.piece(Square::F3) == Board::Piece{Color::BOTH, PieceType::EMPTY_PIECE});
        REQUIRE(board.piece(Square::F4) == Board::Piece{Color::BOTH, PieceType::EMPTY_PIECE});
        REQUIRE(board.piece(Square::F5) == Board::Piece{Color::BOTH, PieceType::EMPTY_PIECE});
        REQUIRE(board.piece(Square::F6) == Board::Piece{Color::BOTH, PieceType::EMPTY_PIECE});
        REQUIRE(board.piece(Square::F7) == Board::Piece{Color::BLACK, PieceType::PAWN});
        REQUIRE(board.piece(Square::F8) == Board::Piece{Color::BLACK, PieceType::BISHOP});

        REQUIRE(board.piece(Square::G1) == Board::Piece{Color::WHITE, PieceType::KNIGHT});
        REQUIRE(board.piece(Square::G2) == Board::Piece{Color::WHITE, PieceType::PAWN});
        REQUIRE(board.piece(Square::G3) == Board::Piece{Color::BOTH, PieceType::EMPTY_PIECE});
        REQUIRE(board.piece(Square::G4) == Board::Piece{Color::BOTH, PieceType::EMPTY_PIECE});
        REQUIRE(board.piece(Square::G5) == Board::Piece{Color::BOTH, PieceType::EMPTY_PIECE});
        REQUIRE(board.piece(Square::G6) == Board::Piece{Color::BOTH, PieceType::EMPTY_PIECE});
        REQUIRE(board.piece(Square::G7) == Board::Piece{Color::BLACK, PieceType::PAWN});
        REQUIRE(board.piece(Square::G8) == Board::Piece{Color::BLACK, PieceType::KNIGHT});

        REQUIRE(board.piece(Square::H1) == Board::Piece{Color::WHITE, PieceType::ROOK});
        REQUIRE(board.piece(Square::H2) == Board::Piece{Color::WHITE, PieceType::PAWN});
        REQUIRE(board.piece(Square::H3) == Board::Piece{Color::BOTH, PieceType::EMPTY_PIECE});
        REQUIRE(board.piece(Square::H4) == Board::Piece{Color::BOTH, PieceType::EMPTY_PIECE});
        REQUIRE(board.piece(Square::H5) == Board::Piece{Color::BOTH, PieceType::EMPTY_PIECE});
        REQUIRE(board.piece(Square::H6) == Board::Piece{Color::BOTH, PieceType::EMPTY_PIECE});
        REQUIRE(board.piece(Square::H7) == Board::Piece{Color::BLACK, PieceType::PAWN});
        REQUIRE(board.piece(Square::H8) == Board::Piece{Color::BLACK, PieceType::ROOK});

        REQUIRE(board.GetFen() == "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1");
    }
}