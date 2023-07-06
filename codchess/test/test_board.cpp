#include <catch2/catch_test_macros.hpp>

#include <iostream>

#include <codchess/codchess.hpp>

using namespace cod;

using chess::Board;
using chess::CastlingAvailability;
using chess::Color;
using chess::Move;
using chess::Piece;
using chess::Square;

TEST_CASE("Board::active() returns the correct color", "[board]") {
    chess::Init();

    SECTION("Position 1") {
        const auto board = Board();
        REQUIRE(board.active() == Color::WHITE);
    }

    SECTION("Position 2") {
        const auto board = Board("8/8/8/8/8/8/8/8 w - - 0 1");
        REQUIRE(board.active() == Color::WHITE);
    }

    SECTION("Position 3") {
        const auto board = Board("8/8/8/8/8/8/8/8 b - - 0 1");
        REQUIRE(board.active() == Color::BLACK);
    }
}

TEST_CASE("Board::inactive() returns the correct color", "[board]") {
    chess::Init();

    SECTION("Position 1") {
        const auto board = Board();
        REQUIRE(board.inactive() == Color::BLACK);
    }

    SECTION("Position 2") {
        const auto board = Board("8/8/8/8/8/8/8/8 w - - 0 1");
        REQUIRE(board.inactive() == Color::BLACK);
    }

    SECTION("Position 3") {
        const auto board = Board("8/8/8/8/8/8/8/8 b - - 0 1");
        REQUIRE(board.inactive() == Color::WHITE);
    }
}

TEST_CASE("Board::castling_availability() returns the correct availability",
          "[board]") {
    chess::Init();

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
        REQUIRE(board.castling_availability() ==
                (CastlingAvailability::WHITE_KING |
                 CastlingAvailability::WHITE_QUEEN |
                 CastlingAvailability::BLACK_KING));
    }

    SECTION("Position 5") {
        const auto board = Board("8/8/8/8/8/8/8/8 w KQq - 0 1");
        REQUIRE(board.castling_availability() ==
                (CastlingAvailability::WHITE_KING |
                 CastlingAvailability::WHITE_QUEEN |
                 CastlingAvailability::BLACK_QUEEN));
    }

    SECTION("Position 6") {
        const auto board = Board("8/8/8/8/8/8/8/8 w Kkq - 0 1");
        REQUIRE(board.castling_availability() ==
                (CastlingAvailability::WHITE_KING |
                 CastlingAvailability::BLACK_KING |
                 CastlingAvailability::BLACK_QUEEN));
    }

    SECTION("Position 7") {
        const auto board = Board("8/8/8/8/8/8/8/8 w Qkq - 0 1");
        REQUIRE(board.castling_availability() ==
                (CastlingAvailability::WHITE_QUEEN |
                 CastlingAvailability::BLACK_KING |
                 CastlingAvailability::BLACK_QUEEN));
    }

    SECTION("Position 8") {
        const auto board = Board("8/8/8/8/8/8/8/8 w Kk - 0 1");
        REQUIRE(board.castling_availability() ==
                (CastlingAvailability::WHITE_KING |
                 CastlingAvailability::BLACK_KING));
    }

    SECTION("Position 9") {
        const auto board = Board("8/8/8/8/8/8/8/8 w Kq - 0 1");
        REQUIRE(board.castling_availability() ==
                (CastlingAvailability::WHITE_KING |
                 CastlingAvailability::BLACK_QUEEN));
    }

    SECTION("Position 10") {
        const auto board = Board("8/8/8/8/8/8/8/8 w Qk - 0 1");
        REQUIRE(board.castling_availability() ==
                (CastlingAvailability::WHITE_QUEEN |
                 CastlingAvailability::BLACK_KING));
    }

    SECTION("Position 11") {
        const auto board = Board("8/8/8/8/8/8/8/8 w Qq - 0 1");
        REQUIRE(board.castling_availability() ==
                (CastlingAvailability::WHITE_QUEEN |
                 CastlingAvailability::BLACK_QUEEN));
    }

    SECTION("Position 12") {
        const auto board = Board("8/8/8/8/8/8/8/8 w K - 0 1");
        REQUIRE(board.castling_availability() ==
                (CastlingAvailability::WHITE_KING));
    }

    SECTION("Position 13") {
        const auto board = Board("8/8/8/8/8/8/8/8 w Q - 0 1");
        REQUIRE(board.castling_availability() ==
                (CastlingAvailability::WHITE_QUEEN));
    }

    SECTION("Position 14") {
        const auto board = Board("8/8/8/8/8/8/8/8 w k - 0 1");
        REQUIRE(board.castling_availability() ==
                (CastlingAvailability::BLACK_KING));
    }

    SECTION("Position 15") {
        const auto board = Board("8/8/8/8/8/8/8/8 w q - 0 1");
        REQUIRE(board.castling_availability() ==
                (CastlingAvailability::BLACK_QUEEN));
    }
}

TEST_CASE("Board::half_move_clock() returns the correct half move clock",
          "[board]") {
    chess::Init();

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

TEST_CASE("Board::en_passant_square() returns the correct en passant square",
          "[board]") {
    chess::Init();

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
    chess::Init();

    SECTION("Position 1") {
        const auto board = Board();

        REQUIRE(board.piece(Square::A1) ==
                Board::ColoredPiece{Color::WHITE, Piece::ROOK});
        REQUIRE(board.piece(Square::B1) ==
                Board::ColoredPiece{Color::WHITE, Piece::KNIGHT});
        REQUIRE(board.piece(Square::C1) ==
                Board::ColoredPiece{Color::WHITE, Piece::BISHOP});
        REQUIRE(board.piece(Square::D1) ==
                Board::ColoredPiece{Color::WHITE, Piece::QUEEN});
        REQUIRE(board.piece(Square::E1) ==
                Board::ColoredPiece{Color::WHITE, Piece::KING});
        REQUIRE(board.piece(Square::F1) ==
                Board::ColoredPiece{Color::WHITE, Piece::BISHOP});
        REQUIRE(board.piece(Square::G1) ==
                Board::ColoredPiece{Color::WHITE, Piece::KNIGHT});
        REQUIRE(board.piece(Square::H1) ==
                Board::ColoredPiece{Color::WHITE, Piece::ROOK});

        REQUIRE(board.piece(Square::A2) ==
                Board::ColoredPiece{Color::WHITE, Piece::PAWN});
        REQUIRE(board.piece(Square::B2) ==
                Board::ColoredPiece{Color::WHITE, Piece::PAWN});
        REQUIRE(board.piece(Square::C2) ==
                Board::ColoredPiece{Color::WHITE, Piece::PAWN});
        REQUIRE(board.piece(Square::D2) ==
                Board::ColoredPiece{Color::WHITE, Piece::PAWN});
        REQUIRE(board.piece(Square::E2) ==
                Board::ColoredPiece{Color::WHITE, Piece::PAWN});
        REQUIRE(board.piece(Square::F2) ==
                Board::ColoredPiece{Color::WHITE, Piece::PAWN});
        REQUIRE(board.piece(Square::G2) ==
                Board::ColoredPiece{Color::WHITE, Piece::PAWN});
        REQUIRE(board.piece(Square::H2) ==
                Board::ColoredPiece{Color::WHITE, Piece::PAWN});

        REQUIRE(board.piece(Square::A3) ==
                Board::ColoredPiece{Color::BLACK, Piece::EMPTY_PIECE});
        REQUIRE(board.piece(Square::B3) ==
                Board::ColoredPiece{Color::BLACK, Piece::EMPTY_PIECE});
        REQUIRE(board.piece(Square::C3) ==
                Board::ColoredPiece{Color::BLACK, Piece::EMPTY_PIECE});
        REQUIRE(board.piece(Square::D3) ==
                Board::ColoredPiece{Color::BLACK, Piece::EMPTY_PIECE});
        REQUIRE(board.piece(Square::E3) ==
                Board::ColoredPiece{Color::BLACK, Piece::EMPTY_PIECE});
        REQUIRE(board.piece(Square::F3) ==
                Board::ColoredPiece{Color::BLACK, Piece::EMPTY_PIECE});
        REQUIRE(board.piece(Square::G3) ==
                Board::ColoredPiece{Color::BLACK, Piece::EMPTY_PIECE});
        REQUIRE(board.piece(Square::H3) ==
                Board::ColoredPiece{Color::BLACK, Piece::EMPTY_PIECE});

        REQUIRE(board.piece(Square::A4) ==
                Board::ColoredPiece{Color::BLACK, Piece::EMPTY_PIECE});
        REQUIRE(board.piece(Square::B4) ==
                Board::ColoredPiece{Color::BLACK, Piece::EMPTY_PIECE});
        REQUIRE(board.piece(Square::C4) ==
                Board::ColoredPiece{Color::BLACK, Piece::EMPTY_PIECE});
        REQUIRE(board.piece(Square::D4) ==
                Board::ColoredPiece{Color::BLACK, Piece::EMPTY_PIECE});
        REQUIRE(board.piece(Square::E4) ==
                Board::ColoredPiece{Color::BLACK, Piece::EMPTY_PIECE});
        REQUIRE(board.piece(Square::F4) ==
                Board::ColoredPiece{Color::BLACK, Piece::EMPTY_PIECE});
        REQUIRE(board.piece(Square::G4) ==
                Board::ColoredPiece{Color::BLACK, Piece::EMPTY_PIECE});
        REQUIRE(board.piece(Square::H4) ==
                Board::ColoredPiece{Color::BLACK, Piece::EMPTY_PIECE});

        REQUIRE(board.piece(Square::A5) ==
                Board::ColoredPiece{Color::BLACK, Piece::EMPTY_PIECE});
        REQUIRE(board.piece(Square::B5) ==
                Board::ColoredPiece{Color::BLACK, Piece::EMPTY_PIECE});
        REQUIRE(board.piece(Square::C5) ==
                Board::ColoredPiece{Color::BLACK, Piece::EMPTY_PIECE});
        REQUIRE(board.piece(Square::D5) ==
                Board::ColoredPiece{Color::BLACK, Piece::EMPTY_PIECE});
        REQUIRE(board.piece(Square::E5) ==
                Board::ColoredPiece{Color::BLACK, Piece::EMPTY_PIECE});
        REQUIRE(board.piece(Square::F5) ==
                Board::ColoredPiece{Color::BLACK, Piece::EMPTY_PIECE});
        REQUIRE(board.piece(Square::G5) ==
                Board::ColoredPiece{Color::BLACK, Piece::EMPTY_PIECE});
        REQUIRE(board.piece(Square::H5) ==
                Board::ColoredPiece{Color::BLACK, Piece::EMPTY_PIECE});

        REQUIRE(board.piece(Square::A6) ==
                Board::ColoredPiece{Color::BLACK, Piece::EMPTY_PIECE});
        REQUIRE(board.piece(Square::B6) ==
                Board::ColoredPiece{Color::BLACK, Piece::EMPTY_PIECE});
        REQUIRE(board.piece(Square::C6) ==
                Board::ColoredPiece{Color::BLACK, Piece::EMPTY_PIECE});
        REQUIRE(board.piece(Square::D6) ==
                Board::ColoredPiece{Color::BLACK, Piece::EMPTY_PIECE});
        REQUIRE(board.piece(Square::E6) ==
                Board::ColoredPiece{Color::BLACK, Piece::EMPTY_PIECE});
        REQUIRE(board.piece(Square::F6) ==
                Board::ColoredPiece{Color::BLACK, Piece::EMPTY_PIECE});
        REQUIRE(board.piece(Square::G6) ==
                Board::ColoredPiece{Color::BLACK, Piece::EMPTY_PIECE});
        REQUIRE(board.piece(Square::H6) ==
                Board::ColoredPiece{Color::BLACK, Piece::EMPTY_PIECE});

        REQUIRE(board.piece(Square::A7) ==
                Board::ColoredPiece{Color::BLACK, Piece::PAWN});
        REQUIRE(board.piece(Square::B7) ==
                Board::ColoredPiece{Color::BLACK, Piece::PAWN});
        REQUIRE(board.piece(Square::C7) ==
                Board::ColoredPiece{Color::BLACK, Piece::PAWN});
        REQUIRE(board.piece(Square::D7) ==
                Board::ColoredPiece{Color::BLACK, Piece::PAWN});
        REQUIRE(board.piece(Square::E7) ==
                Board::ColoredPiece{Color::BLACK, Piece::PAWN});
        REQUIRE(board.piece(Square::F7) ==
                Board::ColoredPiece{Color::BLACK, Piece::PAWN});
        REQUIRE(board.piece(Square::G7) ==
                Board::ColoredPiece{Color::BLACK, Piece::PAWN});
        REQUIRE(board.piece(Square::H7) ==
                Board::ColoredPiece{Color::BLACK, Piece::PAWN});

        REQUIRE(board.piece(Square::A8) ==
                Board::ColoredPiece{Color::BLACK, Piece::ROOK});
        REQUIRE(board.piece(Square::B8) ==
                Board::ColoredPiece{Color::BLACK, Piece::KNIGHT});
        REQUIRE(board.piece(Square::C8) ==
                Board::ColoredPiece{Color::BLACK, Piece::BISHOP});
        REQUIRE(board.piece(Square::D8) ==
                Board::ColoredPiece{Color::BLACK, Piece::QUEEN});
        REQUIRE(board.piece(Square::E8) ==
                Board::ColoredPiece{Color::BLACK, Piece::KING});
        REQUIRE(board.piece(Square::F8) ==
                Board::ColoredPiece{Color::BLACK, Piece::BISHOP});
        REQUIRE(board.piece(Square::G8) ==
                Board::ColoredPiece{Color::BLACK, Piece::KNIGHT});
        REQUIRE(board.piece(Square::H8) ==
                Board::ColoredPiece{Color::BLACK, Piece::ROOK});
    }
}

TEST_CASE("Board::full_move_number() returns the correct full move number",
          "[board]") {
    chess::Init();

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

TEST_CASE("Board::pieces() returns the correct bitboard", "[board]") {
    chess::Init();

    SECTION("Position 1") {
        const auto board = Board();

        REQUIRE(board.pieces(Color::WHITE, Piece::PAWN) ==
                0x000000000000FF00);
        REQUIRE(board.pieces(Color::WHITE, Piece::KNIGHT) ==
                0x0000000000000042);
        REQUIRE(board.pieces(Color::WHITE, Piece::BISHOP) ==
                0x0000000000000024);
        REQUIRE(board.pieces(Color::WHITE, Piece::ROOK) ==
                0x0000000000000081);
        REQUIRE(board.pieces(Color::WHITE, Piece::QUEEN) ==
                0x0000000000000008);
        REQUIRE(board.pieces(Color::WHITE, Piece::KING) ==
                0x0000000000000010);

        REQUIRE(board.pieces(Color::BLACK, Piece::PAWN) ==
                0x00FF000000000000);
        REQUIRE(board.pieces(Color::BLACK, Piece::KNIGHT) ==
                0x4200000000000000);
        REQUIRE(board.pieces(Color::BLACK, Piece::BISHOP) ==
                0x2400000000000000);
        REQUIRE(board.pieces(Color::BLACK, Piece::ROOK) ==
                0x8100000000000000);
        REQUIRE(board.pieces(Color::BLACK, Piece::QUEEN) ==
                0x0800000000000000);
        REQUIRE(board.pieces(Color::BLACK, Piece::KING) ==
                0x1000000000000000);
    }
}

TEST_CASE("Board::occupancies() returns the correct bitboard", "[board]") {
    chess::Init();

    SECTION("Position 1") {
        const auto board = Board();
        REQUIRE(board.occupancies(Color::WHITE) == 0x000000000000FFFF);
        REQUIRE(board.occupancies(Color::BLACK) == 0xFFFF000000000000);
        REQUIRE(board.occupancies(Color::BOTH) == 0xFFFF00000000FFFF);
    }
}

TEST_CASE("Board::GetStateBackup() returns the correct state backup",
          "[board]") {
    chess::Init();

    SECTION("Position 1") {
        const auto board = Board();
        const auto backup = board.GetStateBackup();
        REQUIRE(backup.half_move_clock == 0);
        REQUIRE(backup.full_move_number == 1);
        REQUIRE(backup.en_passant_square == Square::EMPTY_SQUARE);
        REQUIRE(backup.castling_availability == CastlingAvailability::ALL);
    }
}

TEST_CASE("Board::GetFen() returns the correct FEN", "[board]") {
    chess::Init();

    SECTION("Position 1") {
        const auto board = Board();
        REQUIRE(board.ToFen() ==
                "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1");
    }

    SECTION("Position 2") {
        const auto board = Board("8/8/8/8/8/8/8/8 w - - 0 1");
        REQUIRE(board.ToFen() == "8/8/8/8/8/8/8/8 w - - 0 1");
    }

    SECTION("Position 3") {
        const auto board = Board("8/8/8/8/8/8/8/8 w KQkq - 0 1");
        REQUIRE(board.ToFen() == "8/8/8/8/8/8/8/8 w KQkq - 0 1");
    }

    SECTION("Position 4") {
        const auto board = Board("8/8/8/8/8/8/8/8 w - e3 0 1");
        REQUIRE(board.ToFen() == "8/8/8/8/8/8/8/8 w - e3 0 1");
    }

    SECTION("Position 5") {
        const auto board = Board("8/8/8/8/8/8/8/8 w - - 1 1");
        REQUIRE(board.ToFen() == "8/8/8/8/8/8/8/8 w - - 1 1");
    }
}

TEST_CASE("Board::IsSquareAttacked() returns the correct result", "[board]") {
    chess::Init();

    SECTION("Position 1") {
        const auto board = Board();

        for (int square = Square::A3; square <= Square::H3; ++square) {
            REQUIRE(board.IsSquareAttacked(static_cast<Square>(square),
                                           Color::WHITE) == true);
        }

        for (int square = Square::A6; square <= Square::H6; ++square) {
            REQUIRE(board.IsSquareAttacked(static_cast<Square>(square),
                                           Color::BLACK) == true);
        }

        for (int square = Square::A4; square < Square::N_SQUARES; ++square) {
            REQUIRE(board.IsSquareAttacked(static_cast<Square>(square),
                                           Color::WHITE) == false);
        }

        for (int square = Square::A1; square <= Square::H5; ++square) {
            REQUIRE(board.IsSquareAttacked(static_cast<Square>(square),
                                           Color::BLACK) == false);
        }
    }

    SECTION("Position 2") {
        const auto board = Board("8/8/8/8/8/8/8/8 w - - 0 1");
        for (int square = Square::A1; square < Square::N_SQUARES; ++square) {
            REQUIRE(board.IsSquareAttacked(static_cast<Square>(square),
                                           Color::WHITE) == false);
            REQUIRE(board.IsSquareAttacked(static_cast<Square>(square),
                                           Color::BLACK) == false);
        }
    }
}

TEST_CASE("Board::Make() and Board::Unmake() work correctly", "[board]") {
    chess::Init();

    SECTION("Pawn Move") {
        const std::string fen =
            "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1";
        auto board = Board(fen);

        const auto move = Move(Square::E2, Square::E4, Piece::PAWN,
                               Piece::EMPTY_PIECE, Piece::EMPTY_PIECE,
                               true, false, false);
        const auto backup = board.GetStateBackup();

        board.Make(move);
        board.Unmake(move, backup);

        REQUIRE(board.ToFen() == fen);
        REQUIRE(board.hash() == backup.hash);
    }

    SECTION("Knight Capture") {
        const std::string fen =
            "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1";
        auto board = Board(fen);

        const auto move =
            Move(Square::B1, Square::H7, Piece::KNIGHT, Piece::PAWN,
                 Piece::EMPTY_PIECE, false, false, false);
        const auto backup = board.GetStateBackup();

        board.Make(move);
        board.Unmake(move, backup);

        REQUIRE(board.ToFen() == fen);
        REQUIRE(board.hash() == backup.hash);
    }

    SECTION("Castle") {
        const std::string fen =
            "rnbqkbnr/3ppppp/8/ppp5/6P1/5N1B/PPPPPP1P/RNBQK2R w KQkq - 0 4";
        auto board = Board(fen);

        const auto move = Move(Square::E1, Square::G1, Piece::KING,
                               Piece::EMPTY_PIECE, Piece::EMPTY_PIECE,
                               false, false, true);
        const auto backup = board.GetStateBackup();

        board.Make(move);
        board.Unmake(move, backup);

        REQUIRE(board.ToFen() == fen);
        REQUIRE(board.hash() == backup.hash);
    }

    SECTION("Pawn Capture") {
        const std::string fen =
            "rnbqkbnr/3ppppp/8/ppp5/2P3P1/5N1B/PP1PPP1P/RNBQK2R b KQkq - 0 4";
        auto board = Board(fen);

        const auto move =
            Move(Square::B5, Square::C4, Piece::PAWN, Piece::PAWN,
                 Piece::EMPTY_PIECE, false, false, false);
        const auto backup = board.GetStateBackup();

        board.Make(move);
        board.Unmake(move, backup);

        REQUIRE(board.ToFen() == fen);
        REQUIRE(board.hash() == backup.hash);
    }

    SECTION("Pawn Promotion") {
        const std::string fen =
            "4kbnr/P2ppppp/3q4/8/6P1/5N1B/PP1PPP1P/RNBQK1KR w k - 0 4";
        auto board = Board(fen);

        const auto move =
            Move(Square::A7, Square::A8, Piece::PAWN,
                 Piece::EMPTY_PIECE, Piece::QUEEN, false, false, false);
        const auto backup = board.GetStateBackup();

        board.Make(move);
        board.Unmake(move, backup);

        REQUIRE(board.ToFen() == fen);
        REQUIRE(board.hash() == backup.hash);
    }

    SECTION("En Passant Move ") {
        const std::string fen =
            "rnbqkbnr/3ppppp/8/p1p5/2pP2P1/5N1B/PP2PP1P/RNBQK2R b KQkq d3 0 5";
        auto board = Board(fen);

        const auto move =
            Move(Square::C4, Square::D3, Piece::PAWN, Piece::PAWN,
                 Piece::EMPTY_PIECE, false, true, false);
        const auto backup = board.GetStateBackup();

        board.Make(move);
        board.Unmake(move, backup);

        REQUIRE(board.ToFen() == fen);
        REQUIRE(board.hash() == backup.hash);
    }

    SECTION("Double Push") {
        const std::string fen =
            "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1";
        auto board = Board(fen);

        const auto move = Move(Square::D2, Square::D4, Piece::PAWN,
                               Piece::EMPTY_PIECE, Piece::EMPTY_PIECE,
                               true, false, false);
        const auto backup = board.GetStateBackup();

        board.Make(move);
        board.Unmake(move, backup);

        REQUIRE(board.ToFen() == fen);
        REQUIRE(board.hash() == backup.hash);
    }
}