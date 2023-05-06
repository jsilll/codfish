#include <catch2/catch_test_macros.hpp>

#include <iostream>

#include <codchess/codchess.hpp>

TEST_CASE("Board::active() returns the correct color", "[board]") {
    codchess::Init();
    using namespace codchess;

    SECTION("Position 1") {
        const auto board = Board();
        REQUIRE(board.active() == WHITE);
    }

    SECTION("Position 2") {
        const auto board = Board("8/8/8/8/8/8/8/8 w - - 0 1");
        REQUIRE(board.active() == WHITE);
    }

    SECTION("Position 3") {
        const auto board = Board("8/8/8/8/8/8/8/8 b - - 0 1");
        REQUIRE(board.active() == BLACK);
    }
}

TEST_CASE("Board::inactive() returns the correct color", "[board]") {
    codchess::Init();
    using namespace codchess;

    SECTION("Position 1") {
        const auto board = Board();
        REQUIRE(board.inactive() == BLACK);
    }

    SECTION("Position 2") {
        const auto board = Board("8/8/8/8/8/8/8/8 w - - 0 1");
        REQUIRE(board.inactive() == BLACK);
    }

    SECTION("Position 3") {
        const auto board = Board("8/8/8/8/8/8/8/8 b - - 0 1");
        REQUIRE(board.inactive() == WHITE);
    }
}

TEST_CASE("Board::castling_availability() returns the correct availability",
          "[board]") {
    codchess::Init();
    using namespace codchess;

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
        const auto board = Board("8/8/8/8/8/8/8/8 w q- 0 1");
        REQUIRE(board.castling_availability() ==
                (CastlingAvailability::BLACK_QUEEN));
    }
}

TEST_CASE("Board::half_move_clock() returns the correct half move clock",
          "[board]") {
    codchess::Init();
    using namespace codchess;

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
    codchess::Init();
    using namespace codchess;

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
    codchess::Init();
    using namespace codchess;

    SECTION("Position 1") {
        const auto board = Board();

        REQUIRE(board.piece(Square::A1) ==
                Board::Piece{WHITE, ROOK});
        REQUIRE(board.piece(Square::B1) ==
                Board::Piece{WHITE, KNIGHT});
        REQUIRE(board.piece(Square::C1) ==
                Board::Piece{WHITE, BISHOP});
        REQUIRE(board.piece(Square::D1) ==
                Board::Piece{WHITE, QUEEN});
        REQUIRE(board.piece(Square::E1) ==
                Board::Piece{WHITE, KING});
        REQUIRE(board.piece(Square::F1) ==
                Board::Piece{WHITE, BISHOP});
        REQUIRE(board.piece(Square::G1) ==
                Board::Piece{WHITE, KNIGHT});
        REQUIRE(board.piece(Square::H1) ==
                Board::Piece{WHITE, ROOK});

        REQUIRE(board.piece(Square::A2) ==
                Board::Piece{WHITE, PAWN});
        REQUIRE(board.piece(Square::B2) ==
                Board::Piece{WHITE, PAWN});
        REQUIRE(board.piece(Square::C2) ==
                Board::Piece{WHITE, PAWN});
        REQUIRE(board.piece(Square::D2) ==
                Board::Piece{WHITE, PAWN});
        REQUIRE(board.piece(Square::E2) ==
                Board::Piece{WHITE, PAWN});
        REQUIRE(board.piece(Square::F2) ==
                Board::Piece{WHITE, PAWN});
        REQUIRE(board.piece(Square::G2) ==
                Board::Piece{WHITE, PAWN});
        REQUIRE(board.piece(Square::H2) ==
                Board::Piece{WHITE, PAWN});

        REQUIRE(board.piece(Square::A3) ==
                Board::Piece{BLACK, EMPTY_PIECE});
        REQUIRE(board.piece(Square::B3) ==
                Board::Piece{BLACK, EMPTY_PIECE});
        REQUIRE(board.piece(Square::C3) ==
                Board::Piece{BLACK, EMPTY_PIECE});
        REQUIRE(board.piece(Square::D3) ==
                Board::Piece{BLACK, EMPTY_PIECE});
        REQUIRE(board.piece(Square::E3) ==
                Board::Piece{BLACK, EMPTY_PIECE});
        REQUIRE(board.piece(Square::F3) ==
                Board::Piece{BLACK, EMPTY_PIECE});
        REQUIRE(board.piece(Square::G3) ==
                Board::Piece{BLACK, EMPTY_PIECE});
        REQUIRE(board.piece(Square::H3) ==
                Board::Piece{BLACK, EMPTY_PIECE});

        REQUIRE(board.piece(Square::A4) ==
                Board::Piece{BLACK, EMPTY_PIECE});
        REQUIRE(board.piece(Square::B4) ==
                Board::Piece{BLACK, EMPTY_PIECE});
        REQUIRE(board.piece(Square::C4) ==
                Board::Piece{BLACK, EMPTY_PIECE});
        REQUIRE(board.piece(Square::D4) ==
                Board::Piece{BLACK, EMPTY_PIECE});
        REQUIRE(board.piece(Square::E4) ==
                Board::Piece{BLACK, EMPTY_PIECE});
        REQUIRE(board.piece(Square::F4) ==
                Board::Piece{BLACK, EMPTY_PIECE});
        REQUIRE(board.piece(Square::G4) ==
                Board::Piece{BLACK, EMPTY_PIECE});
        REQUIRE(board.piece(Square::H4) ==
                Board::Piece{BLACK, EMPTY_PIECE});

        REQUIRE(board.piece(Square::A5) ==
                Board::Piece{BLACK, EMPTY_PIECE});
        REQUIRE(board.piece(Square::B5) ==
                Board::Piece{BLACK, EMPTY_PIECE});
        REQUIRE(board.piece(Square::C5) ==
                Board::Piece{BLACK, EMPTY_PIECE});
        REQUIRE(board.piece(Square::D5) ==
                Board::Piece{BLACK, EMPTY_PIECE});
        REQUIRE(board.piece(Square::E5) ==
                Board::Piece{BLACK, EMPTY_PIECE});
        REQUIRE(board.piece(Square::F5) ==
                Board::Piece{BLACK, EMPTY_PIECE});
        REQUIRE(board.piece(Square::G5) ==
                Board::Piece{BLACK, EMPTY_PIECE});
        REQUIRE(board.piece(Square::H5) ==
                Board::Piece{BLACK, EMPTY_PIECE});

        REQUIRE(board.piece(Square::A6) ==
                Board::Piece{BLACK, EMPTY_PIECE});
        REQUIRE(board.piece(Square::B6) ==
                Board::Piece{BLACK, EMPTY_PIECE});
        REQUIRE(board.piece(Square::C6) ==
                Board::Piece{BLACK, EMPTY_PIECE});
        REQUIRE(board.piece(Square::D6) ==
                Board::Piece{BLACK, EMPTY_PIECE});
        REQUIRE(board.piece(Square::E6) ==
                Board::Piece{BLACK, EMPTY_PIECE});
        REQUIRE(board.piece(Square::F6) ==
                Board::Piece{BLACK, EMPTY_PIECE});
        REQUIRE(board.piece(Square::G6) ==
                Board::Piece{BLACK, EMPTY_PIECE});
        REQUIRE(board.piece(Square::H6) ==
                Board::Piece{BLACK, EMPTY_PIECE});

        REQUIRE(board.piece(Square::A7) ==
                Board::Piece{BLACK, PAWN});
        REQUIRE(board.piece(Square::B7) ==
                Board::Piece{BLACK, PAWN});
        REQUIRE(board.piece(Square::C7) ==
                Board::Piece{BLACK, PAWN});
        REQUIRE(board.piece(Square::D7) ==
                Board::Piece{BLACK, PAWN});
        REQUIRE(board.piece(Square::E7) ==
                Board::Piece{BLACK, PAWN});
        REQUIRE(board.piece(Square::F7) ==
                Board::Piece{BLACK, PAWN});
        REQUIRE(board.piece(Square::G7) ==
                Board::Piece{BLACK, PAWN});
        REQUIRE(board.piece(Square::H7) ==
                Board::Piece{BLACK, PAWN});

        REQUIRE(board.piece(Square::A8) ==
                Board::Piece{BLACK, ROOK});
        REQUIRE(board.piece(Square::B8) ==
                Board::Piece{BLACK, KNIGHT});
        REQUIRE(board.piece(Square::C8) ==
                Board::Piece{BLACK, BISHOP});
        REQUIRE(board.piece(Square::D8) ==
                Board::Piece{BLACK, QUEEN});
        REQUIRE(board.piece(Square::E8) ==
                Board::Piece{BLACK, KING});
        REQUIRE(board.piece(Square::F8) ==
                Board::Piece{BLACK, BISHOP});
        REQUIRE(board.piece(Square::G8) ==
                Board::Piece{BLACK, KNIGHT});
        REQUIRE(board.piece(Square::H8) ==
                Board::Piece{BLACK, ROOK});
    }
}

TEST_CASE("Board::full_move_number() returns the correct full move number",
          "[board]") {
    codchess::Init();
    using namespace codchess;

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
    codchess::Init();
    using namespace codchess;

    SECTION("Position 1") {
        const auto board = Board();

        REQUIRE(board.pieces(WHITE, PAWN) ==
                0x000000000000FF00);
        REQUIRE(board.pieces(WHITE, KNIGHT) ==
                0x0000000000000042);
        REQUIRE(board.pieces(WHITE, BISHOP) ==
                0x0000000000000024);
        REQUIRE(board.pieces(WHITE, ROOK) ==
                0x0000000000000081);
        REQUIRE(board.pieces(WHITE, QUEEN) ==
                0x0000000000000008);
        REQUIRE(board.pieces(WHITE, KING) ==
                0x0000000000000010);

        REQUIRE(board.pieces(BLACK, PAWN) ==
                0x00FF000000000000);
        REQUIRE(board.pieces(BLACK, KNIGHT) ==
                0x4200000000000000);
        REQUIRE(board.pieces(BLACK, BISHOP) ==
                0x2400000000000000);
        REQUIRE(board.pieces(BLACK, ROOK) ==
                0x8100000000000000);
        REQUIRE(board.pieces(BLACK, QUEEN) ==
                0x0800000000000000);
        REQUIRE(board.pieces(BLACK, KING) ==
                0x1000000000000000);
    }
}

TEST_CASE("Board::occupancies() returns the correct bitboard", "[board]") {
    codchess::Init();
    using namespace codchess;

    SECTION("Position 1") {
        const auto board = Board();
        REQUIRE(board.occupancies(WHITE) == 0x000000000000FFFF);
        REQUIRE(board.occupancies(BLACK) == 0xFFFF000000000000);
        REQUIRE(board.occupancies(BOTH) == 0xFFFF00000000FFFF);
    }
}

TEST_CASE("Board::GetStateBackup() returns the correct state backup",
          "[board]") {
    codchess::Init();
    using namespace codchess;

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
    codchess::Init();
    using namespace codchess;

    SECTION("Position 1") {
        const auto board = Board();
        REQUIRE(board.GetFen() ==
                "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1");
    }

    SECTION("Position 2") {
        const auto board = Board("8/8/8/8/8/8/8/8 w - - 0 1");
        REQUIRE(board.GetFen() == "8/8/8/8/8/8/8/8 w - - 0 1");
    }

    SECTION("Position 3") {
        const auto board = Board("8/8/8/8/8/8/8/8 w KQkq - 0 1");
        REQUIRE(board.GetFen() == "8/8/8/8/8/8/8/8 w KQkq - 0 1");
    }

    SECTION("Position 4") {
        const auto board = Board("8/8/8/8/8/8/8/8 w - e3 0 1");
        REQUIRE(board.GetFen() == "8/8/8/8/8/8/8/8 w - e3 0 1");
    }

    SECTION("Position 5") {
        const auto board = Board("8/8/8/8/8/8/8/8 w - - 1 1");
        REQUIRE(board.GetFen() == "8/8/8/8/8/8/8/8 w - - 1 1");
    }
}

TEST_CASE("Board::IsSquareAttacked() returns the correct result", "[board]") {
    codchess::Init();
    using namespace codchess;

    SECTION("Position 1") {
        const auto board = Board();

        for (int square = A3; square <= H3; ++square) {
            REQUIRE(board.IsSquareAttacked(static_cast<Square>(square),
                                           WHITE) == true);
        }

        for (int square = A6; square <= H6; ++square) {
            REQUIRE(board.IsSquareAttacked(static_cast<Square>(square),
                                           BLACK) == true);
        }

        for (int square = A4; square < N_SQUARES; ++square) {
            REQUIRE(board.IsSquareAttacked(static_cast<Square>(square),
                                           WHITE) == false);
        }

        for (int square = A1; square <= H5; ++square) {
            REQUIRE(board.IsSquareAttacked(static_cast<Square>(square),
                                           BLACK) == false);
        }
    }

    SECTION("Position 2") {
        const auto board = Board("8/8/8/8/8/8/8/8 w - - 0 1");
        for (int square = 0; square < 64; ++square) {
            REQUIRE(board.IsSquareAttacked(static_cast<Square>(square),
                                           WHITE) == false);
            REQUIRE(board.IsSquareAttacked(static_cast<Square>(square),
                                           BLACK) == false);
        }
    }
}

TEST_CASE("Board::Make() and Board::Unmake() work correctly", "[board]") {
    codchess::Init();
    using namespace codchess;

    SECTION("Pawn Move") {
        const std::string fen =
            "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1";
        auto board = Board(fen);

        const auto move =
            Move(E2, E4, PAWN, EMPTY_PIECE, EMPTY_PIECE, true, false, false);
        const auto backup = board.GetStateBackup();

        board.Make(move);
        board.Unmake(move, backup);

        REQUIRE(board.GetFen() == fen);
        REQUIRE(board.hash() == backup.hash);
    }

    SECTION("Knight Capture") {
        const std::string fen =
            "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1";
        auto board = Board(fen);

        const auto move =
            Move(B1, H7, KNIGHT, PAWN, EMPTY_PIECE, false, false, false);
        const auto backup = board.GetStateBackup();

        board.Make(move);
        board.Unmake(move, backup);

        REQUIRE(board.GetFen() == fen);
        REQUIRE(board.hash() == backup.hash);
    }

    SECTION("Castle") {
        const std::string fen =
            "rnbqkbnr/3ppppp/8/ppp5/6P1/5N1B/PPPPPP1P/RNBQK2R w KQkq - 0 4";
        auto board = Board(fen);

        const auto move =
            Move(E1, G1, KING, EMPTY_PIECE, EMPTY_PIECE, false, false, true);
        const auto backup = board.GetStateBackup();

        board.Make(move);
        board.Unmake(move, backup);

        REQUIRE(board.GetFen() == fen);
        REQUIRE(board.hash() == backup.hash);
    }

    SECTION("Pawn Capture") {
        const std::string fen =
            "rnbqkbnr/3ppppp/8/ppp5/2P3P1/5N1B/PP1PPP1P/RNBQK2R b KQkq - 0 4";
        auto board = Board(fen);

        const auto move =
            Move(B5, C4, PAWN, PAWN, EMPTY_PIECE, false, false, false);
        const auto backup = board.GetStateBackup();

        board.Make(move);
        board.Unmake(move, backup);

        REQUIRE(board.GetFen() == fen);
        REQUIRE(board.hash() == backup.hash);
    }

    SECTION("Pawn Promotion") {
        const std::string fen =
            "4kbnr/P2ppppp/3q4/8/6P1/5N1B/PP1PPP1P/RNBQK1KR w k - 0 4";
        auto board = Board(fen);

        const auto move =
            Move(A7, A8, PAWN, EMPTY_PIECE, QUEEN, false, false, false);
        const auto backup = board.GetStateBackup();

        board.Make(move);
        board.Unmake(move, backup);

        REQUIRE(board.GetFen() == fen);
        REQUIRE(board.hash() == backup.hash);
    }

    SECTION("En Passant Move ") {
        const std::string fen =
            "rnbqkbnr/3ppppp/8/p1p5/2pP2P1/5N1B/PP2PP1P/RNBQK2R b KQkq d3 0 5";
        auto board = Board(fen);

        const auto move =
            Move(C4, D3, PAWN, PAWN, EMPTY_PIECE, false, true, false);
        const auto backup = board.GetStateBackup();

        board.Make(move);
        board.Unmake(move, backup);

        REQUIRE(board.GetFen() == fen);
        REQUIRE(board.hash() == backup.hash);
    }

    SECTION("Double Push") {
        const std::string fen =
            "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1";
        auto board = Board(fen);

        const auto move =
            Move(D2, D4, PAWN, EMPTY_PIECE, EMPTY_PIECE, true, false, false);
        const auto backup = board.GetStateBackup();

        board.Make(move);
        board.Unmake(move, backup);

        REQUIRE(board.GetFen() == fen);
        REQUIRE(board.hash() == backup.hash);
    }
}