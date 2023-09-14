#include <catch2/catch_test_macros.hpp>

#include <iostream>

#include <core/codchess.hpp>

using namespace codfish;

using core::Board;
using core::Castle;
using core::Color;
using core::Move;
using core::Piece;
using core::Square;

TEST_CASE("Board::active() returns the correct color", "[board]") {
    core::Init();

    SECTION("Position 1") {
        const auto board = Board();
        REQUIRE(board.active() == Color::White);
    }

    SECTION("Position 2") {
        const auto board = Board("8/8/8/8/8/8/8/8 w - - 0 1");
        REQUIRE(board.active() == Color::White);
    }

    SECTION("Position 3") {
        const auto board = Board("8/8/8/8/8/8/8/8 b - - 0 1");
        REQUIRE(board.active() == Color::Black);
    }
}

TEST_CASE("Board::inactive() returns the correct color", "[board]") {
    core::Init();

    SECTION("Position 1") {
        const auto board = Board();
        REQUIRE(board.inactive() == Color::Black);
    }

    SECTION("Position 2") {
        const auto board = Board("8/8/8/8/8/8/8/8 w - - 0 1");
        REQUIRE(board.inactive() == Color::Black);
    }

    SECTION("Position 3") {
        const auto board = Board("8/8/8/8/8/8/8/8 b - - 0 1");
        REQUIRE(board.inactive() == Color::White);
    }
}

TEST_CASE("Board::castling_availability() returns the correct availability",
          "[board]") {
    core::Init();

    SECTION("Position 1") {
        const auto board = Board();
        REQUIRE(board.castling_availability() == Castle::All);
    }

    SECTION("Position 2") {
        const auto board = Board("8/8/8/8/8/8/8/8 w - - 0 1");
        REQUIRE(board.castling_availability() == Castle::None);
    }

    SECTION("Position 3") {
        const auto board = Board("8/8/8/8/8/8/8/8 w KQkq - 0 1");
        REQUIRE(board.castling_availability() == Castle::All);
    }

    SECTION("Position 4") {
        const auto board = Board("8/8/8/8/8/8/8/8 w KQk - 0 1");
        const auto expected = static_cast<Castle>(
            static_cast<std::uint8_t>(Castle::WhiteKing) |
            static_cast<std::uint8_t>(Castle::WhiteQueen) |
            static_cast<std::uint8_t>(Castle::BlackKing));
        REQUIRE(board.castling_availability() == expected);
    }

    SECTION("Position 5") {
        const auto board = Board("8/8/8/8/8/8/8/8 w KQq - 0 1");
        const auto expected = static_cast<Castle>(
            static_cast<std::uint8_t>(Castle::WhiteKing) |
            static_cast<std::uint8_t>(Castle::WhiteQueen) |
            static_cast<std::uint8_t>(Castle::BlackQueen));
        REQUIRE(board.castling_availability() == expected);
    }

    SECTION("Position 6") {
        const auto board = Board("8/8/8/8/8/8/8/8 w Kkq - 0 1");
        const auto expected = static_cast<Castle>(
            static_cast<std::uint8_t>(Castle::WhiteKing) |
            static_cast<std::uint8_t>(Castle::BlackKing) |
            static_cast<std::uint8_t>(Castle::BlackQueen));
        REQUIRE(board.castling_availability() == expected);
    }

    SECTION("Position 7") {
        const auto board = Board("8/8/8/8/8/8/8/8 w Qkq - 0 1");
        const auto expected = static_cast<Castle>(
            static_cast<std::uint8_t>(Castle::WhiteQueen) |
            static_cast<std::uint8_t>(Castle::BlackKing) |
            static_cast<std::uint8_t>(Castle::BlackQueen));
        REQUIRE(board.castling_availability() == expected);
    }

    SECTION("Position 8") {
        const auto board = Board("8/8/8/8/8/8/8/8 w Kk - 0 1");
        const auto expected = static_cast<Castle>(
            static_cast<std::uint8_t>(Castle::WhiteKing) |
            static_cast<std::uint8_t>(Castle::BlackKing));
        REQUIRE(board.castling_availability() == expected);
    }

    SECTION("Position 9") {
        const auto board = Board("8/8/8/8/8/8/8/8 w Kq - 0 1");
        const auto expected = static_cast<Castle>(
            static_cast<std::uint8_t>(Castle::WhiteKing) |
            static_cast<std::uint8_t>(Castle::BlackQueen));
        REQUIRE(board.castling_availability() == expected);
    }

    SECTION("Position 10") {
        const auto board = Board("8/8/8/8/8/8/8/8 w Qk - 0 1");
        const auto expected = static_cast<Castle>(
            static_cast<std::uint8_t>(Castle::WhiteQueen) |
            static_cast<std::uint8_t>(Castle::BlackKing));
        REQUIRE(board.castling_availability() == expected);
    }

    SECTION("Position 11") {
        const auto board = Board("8/8/8/8/8/8/8/8 w Qq - 0 1");
        const auto expected = static_cast<Castle>(
            static_cast<std::uint8_t>(Castle::WhiteQueen) |
            static_cast<std::uint8_t>(Castle::BlackQueen));
        REQUIRE(board.castling_availability() == expected);
    }

    SECTION("Position 12") {
        const auto board = Board("8/8/8/8/8/8/8/8 w K - 0 1");
        REQUIRE(board.castling_availability() ==
                (Castle::WhiteKing));
    }

    SECTION("Position 13") {
        const auto board = Board("8/8/8/8/8/8/8/8 w Q - 0 1");
        REQUIRE(board.castling_availability() ==
                (Castle::WhiteQueen));
    }

    SECTION("Position 14") {
        const auto board = Board("8/8/8/8/8/8/8/8 w k - 0 1");
        REQUIRE(board.castling_availability() ==
                (Castle::BlackKing));
    }

    SECTION("Position 15") {
        const auto board = Board("8/8/8/8/8/8/8/8 w q - 0 1");
        REQUIRE(board.castling_availability() ==
                (Castle::BlackQueen));
    }
}

TEST_CASE("Board::half_move_clock() returns the correct half move clock",
          "[board]") {
    core::Init();

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
    core::Init();

    SECTION("Position 1") {
        const auto board = Board();
        REQUIRE(board.en_passant_square() == Square::Empty);
    }

    SECTION("Position 2") {
        const auto board = Board("8/8/8/8/8/8/8/8 w - - 0 1");
        REQUIRE(board.en_passant_square() == Square::Empty);
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
    core::Init();

    SECTION("Position 1") {
        const auto board = Board();

        REQUIRE(board.piece(Square::A1) ==
                Board::ColoredPiece{Color::White, Piece::Rook});
        REQUIRE(board.piece(Square::B1) ==
                Board::ColoredPiece{Color::White, Piece::Knight});
        REQUIRE(board.piece(Square::C1) ==
                Board::ColoredPiece{Color::White, Piece::Bishop});
        REQUIRE(board.piece(Square::D1) ==
                Board::ColoredPiece{Color::White, Piece::Queen});
        REQUIRE(board.piece(Square::E1) ==
                Board::ColoredPiece{Color::White, Piece::King});
        REQUIRE(board.piece(Square::F1) ==
                Board::ColoredPiece{Color::White, Piece::Bishop});
        REQUIRE(board.piece(Square::G1) ==
                Board::ColoredPiece{Color::White, Piece::Knight});
        REQUIRE(board.piece(Square::H1) ==
                Board::ColoredPiece{Color::White, Piece::Rook});

        REQUIRE(board.piece(Square::A2) ==
                Board::ColoredPiece{Color::White, Piece::Pawn});
        REQUIRE(board.piece(Square::B2) ==
                Board::ColoredPiece{Color::White, Piece::Pawn});
        REQUIRE(board.piece(Square::C2) ==
                Board::ColoredPiece{Color::White, Piece::Pawn});
        REQUIRE(board.piece(Square::D2) ==
                Board::ColoredPiece{Color::White, Piece::Pawn});
        REQUIRE(board.piece(Square::E2) ==
                Board::ColoredPiece{Color::White, Piece::Pawn});
        REQUIRE(board.piece(Square::F2) ==
                Board::ColoredPiece{Color::White, Piece::Pawn});
        REQUIRE(board.piece(Square::G2) ==
                Board::ColoredPiece{Color::White, Piece::Pawn});
        REQUIRE(board.piece(Square::H2) ==
                Board::ColoredPiece{Color::White, Piece::Pawn});

        REQUIRE(board.piece(Square::A3) ==
                Board::ColoredPiece{Color::Black, Piece::Empty});
        REQUIRE(board.piece(Square::B3) ==
                Board::ColoredPiece{Color::Black, Piece::Empty});
        REQUIRE(board.piece(Square::C3) ==
                Board::ColoredPiece{Color::Black, Piece::Empty});
        REQUIRE(board.piece(Square::D3) ==
                Board::ColoredPiece{Color::Black, Piece::Empty});
        REQUIRE(board.piece(Square::E3) ==
                Board::ColoredPiece{Color::Black, Piece::Empty});
        REQUIRE(board.piece(Square::F3) ==
                Board::ColoredPiece{Color::Black, Piece::Empty});
        REQUIRE(board.piece(Square::G3) ==
                Board::ColoredPiece{Color::Black, Piece::Empty});
        REQUIRE(board.piece(Square::H3) ==
                Board::ColoredPiece{Color::Black, Piece::Empty});

        REQUIRE(board.piece(Square::A4) ==
                Board::ColoredPiece{Color::Black, Piece::Empty});
        REQUIRE(board.piece(Square::B4) ==
                Board::ColoredPiece{Color::Black, Piece::Empty});
        REQUIRE(board.piece(Square::C4) ==
                Board::ColoredPiece{Color::Black, Piece::Empty});
        REQUIRE(board.piece(Square::D4) ==
                Board::ColoredPiece{Color::Black, Piece::Empty});
        REQUIRE(board.piece(Square::E4) ==
                Board::ColoredPiece{Color::Black, Piece::Empty});
        REQUIRE(board.piece(Square::F4) ==
                Board::ColoredPiece{Color::Black, Piece::Empty});
        REQUIRE(board.piece(Square::G4) ==
                Board::ColoredPiece{Color::Black, Piece::Empty});
        REQUIRE(board.piece(Square::H4) ==
                Board::ColoredPiece{Color::Black, Piece::Empty});

        REQUIRE(board.piece(Square::A5) ==
                Board::ColoredPiece{Color::Black, Piece::Empty});
        REQUIRE(board.piece(Square::B5) ==
                Board::ColoredPiece{Color::Black, Piece::Empty});
        REQUIRE(board.piece(Square::C5) ==
                Board::ColoredPiece{Color::Black, Piece::Empty});
        REQUIRE(board.piece(Square::D5) ==
                Board::ColoredPiece{Color::Black, Piece::Empty});
        REQUIRE(board.piece(Square::E5) ==
                Board::ColoredPiece{Color::Black, Piece::Empty});
        REQUIRE(board.piece(Square::F5) ==
                Board::ColoredPiece{Color::Black, Piece::Empty});
        REQUIRE(board.piece(Square::G5) ==
                Board::ColoredPiece{Color::Black, Piece::Empty});
        REQUIRE(board.piece(Square::H5) ==
                Board::ColoredPiece{Color::Black, Piece::Empty});

        REQUIRE(board.piece(Square::A6) ==
                Board::ColoredPiece{Color::Black, Piece::Empty});
        REQUIRE(board.piece(Square::B6) ==
                Board::ColoredPiece{Color::Black, Piece::Empty});
        REQUIRE(board.piece(Square::C6) ==
                Board::ColoredPiece{Color::Black, Piece::Empty});
        REQUIRE(board.piece(Square::D6) ==
                Board::ColoredPiece{Color::Black, Piece::Empty});
        REQUIRE(board.piece(Square::E6) ==
                Board::ColoredPiece{Color::Black, Piece::Empty});
        REQUIRE(board.piece(Square::F6) ==
                Board::ColoredPiece{Color::Black, Piece::Empty});
        REQUIRE(board.piece(Square::G6) ==
                Board::ColoredPiece{Color::Black, Piece::Empty});
        REQUIRE(board.piece(Square::H6) ==
                Board::ColoredPiece{Color::Black, Piece::Empty});

        REQUIRE(board.piece(Square::A7) ==
                Board::ColoredPiece{Color::Black, Piece::Pawn});
        REQUIRE(board.piece(Square::B7) ==
                Board::ColoredPiece{Color::Black, Piece::Pawn});
        REQUIRE(board.piece(Square::C7) ==
                Board::ColoredPiece{Color::Black, Piece::Pawn});
        REQUIRE(board.piece(Square::D7) ==
                Board::ColoredPiece{Color::Black, Piece::Pawn});
        REQUIRE(board.piece(Square::E7) ==
                Board::ColoredPiece{Color::Black, Piece::Pawn});
        REQUIRE(board.piece(Square::F7) ==
                Board::ColoredPiece{Color::Black, Piece::Pawn});
        REQUIRE(board.piece(Square::G7) ==
                Board::ColoredPiece{Color::Black, Piece::Pawn});
        REQUIRE(board.piece(Square::H7) ==
                Board::ColoredPiece{Color::Black, Piece::Pawn});

        REQUIRE(board.piece(Square::A8) ==
                Board::ColoredPiece{Color::Black, Piece::Rook});
        REQUIRE(board.piece(Square::B8) ==
                Board::ColoredPiece{Color::Black, Piece::Knight});
        REQUIRE(board.piece(Square::C8) ==
                Board::ColoredPiece{Color::Black, Piece::Bishop});
        REQUIRE(board.piece(Square::D8) ==
                Board::ColoredPiece{Color::Black, Piece::Queen});
        REQUIRE(board.piece(Square::E8) ==
                Board::ColoredPiece{Color::Black, Piece::King});
        REQUIRE(board.piece(Square::F8) ==
                Board::ColoredPiece{Color::Black, Piece::Bishop});
        REQUIRE(board.piece(Square::G8) ==
                Board::ColoredPiece{Color::Black, Piece::Knight});
        REQUIRE(board.piece(Square::H8) ==
                Board::ColoredPiece{Color::Black, Piece::Rook});
    }
}

TEST_CASE("Board::full_move_number() returns the correct full move number",
          "[board]") {
    core::Init();

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
    core::Init();

    SECTION("Position 1") {
        const auto board = Board();

        REQUIRE(board.pieces(Color::White, Piece::Pawn) == 0x000000000000FF00);
        REQUIRE(board.pieces(Color::White, Piece::Knight) ==
                0x0000000000000042);
        REQUIRE(board.pieces(Color::White, Piece::Bishop) ==
                0x0000000000000024);
        REQUIRE(board.pieces(Color::White, Piece::Rook) == 0x0000000000000081);
        REQUIRE(board.pieces(Color::White, Piece::Queen) == 0x0000000000000008);
        REQUIRE(board.pieces(Color::White, Piece::King) == 0x0000000000000010);

        REQUIRE(board.pieces(Color::Black, Piece::Pawn) == 0x00FF000000000000);
        REQUIRE(board.pieces(Color::Black, Piece::Knight) ==
                0x4200000000000000);
        REQUIRE(board.pieces(Color::Black, Piece::Bishop) ==
                0x2400000000000000);
        REQUIRE(board.pieces(Color::Black, Piece::Rook) == 0x8100000000000000);
        REQUIRE(board.pieces(Color::Black, Piece::Queen) == 0x0800000000000000);
        REQUIRE(board.pieces(Color::Black, Piece::King) == 0x1000000000000000);
    }
}

TEST_CASE("Board::occupancies() returns the correct bitboard", "[board]") {
    core::Init();

    SECTION("Position 1") {
        const auto board = Board();
        REQUIRE(board.occupancies(Color::White) == 0x000000000000FFFF);
        REQUIRE(board.occupancies(Color::Black) == 0xFFFF000000000000);
        REQUIRE(board.occupancies(Color::Both) == 0xFFFF00000000FFFF);
    }
}

TEST_CASE("Board::GetStateBackup() returns the correct state backup",
          "[board]") {
    core::Init();

    SECTION("Position 1") {
        const auto board = Board();
        const auto backup = board.GetStateBackup();
        REQUIRE(backup.half_move_clock == 0);
        REQUIRE(backup.full_move_number == 1);
        REQUIRE(backup.en_passant_square == Square::Empty);
        REQUIRE(backup.castling_availability == Castle::All);
    }
}

TEST_CASE("Board::GetFen() returns the correct FEN", "[board]") {
    core::Init();

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
    core::Init();

    SECTION("Position 1") {
        const auto board = Board();

        for (int square = static_cast<int>(Square::A3);
             square <= static_cast<int>(Square::H3); ++square) {
            REQUIRE(board.IsSquareAttacked(static_cast<Square>(square),
                                           Color::White) == true);
        }

        for (int square = static_cast<int>(Square::A6);
             square <= static_cast<int>(Square::H6); ++square) {
            REQUIRE(board.IsSquareAttacked(static_cast<Square>(square),
                                           Color::Black) == true);
        }

        for (int square = static_cast<int>(Square::A4);
             square < static_cast<int>(Square::Total); ++square) {
            REQUIRE(board.IsSquareAttacked(static_cast<Square>(square),
                                           Color::White) == false);
        }

        for (int square = static_cast<int>(Square::A1);
             square <= static_cast<int>(Square::H5); ++square) {
            REQUIRE(board.IsSquareAttacked(static_cast<Square>(square),
                                           Color::Black) == false);
        }
    }

    SECTION("Position 2") {
        const auto board = Board("8/8/8/8/8/8/8/8 w - - 0 1");
        for (int square = static_cast<int>(Square::A1);
             square < static_cast<int>(Square::Total); ++square) {
            REQUIRE(board.IsSquareAttacked(static_cast<Square>(square),
                                           Color::White) == false);
            REQUIRE(board.IsSquareAttacked(static_cast<Square>(square),
                                           Color::Black) == false);
        }
    }
}

TEST_CASE("Board::Make() and Board::Unmake() work correctly", "[board]") {
    core::Init();

    SECTION("Pawn Move") {
        const std::string fen =
            "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1";
        auto board = Board(fen);

        const auto move =
            Move(Square::E2, Square::E4, Piece::Pawn, Piece::Empty,
                 Piece::Empty, true, false, false);
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
            Move(Square::B1, Square::H7, Piece::Knight, Piece::Pawn,
                 Piece::Empty, false, false, false);
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

        const auto move =
            Move(Square::E1, Square::G1, Piece::King, Piece::Empty,
                 Piece::Empty, false, false, true);
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

        const auto move = Move(Square::B5, Square::C4, Piece::Pawn, Piece::Pawn,
                               Piece::Empty, false, false, false);
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
            Move(Square::A7, Square::A8, Piece::Pawn, Piece::Empty,
                 Piece::Queen, false, false, false);
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

        const auto move = Move(Square::C4, Square::D3, Piece::Pawn, Piece::Pawn,
                               Piece::Empty, false, true, false);
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

        const auto move =
            Move(Square::D2, Square::D4, Piece::Pawn, Piece::Empty,
                 Piece::Empty, true, false, false);
        const auto backup = board.GetStateBackup();

        board.Make(move);
        board.Unmake(move, backup);

        REQUIRE(board.ToFen() == fen);
        REQUIRE(board.hash() == backup.hash);
    }
}