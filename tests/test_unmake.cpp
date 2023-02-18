#include <catch2/catch_test_macros.hpp>

#include <codlib/codlib.hpp>

TEST_CASE("Unmake move")
{
    codlib::init();

    SECTION("PAWNS rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1")
    {
        auto board = Board("1k6/p6p/K6P/8/8/8/8/1q4q1 b - - 0 1");
        Move move = Move(E2, E4, PAWN, EMPTY_PIECE, EMPTY_PIECE, true, false, false);
        Board::StateBackup state = board.GetStateBackup();
        board.Make(move);
        board.Unmake(move, state);
        REQUIRE(board.GetFen() == "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1\n");
    }

    SECTION("knight capture rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1")
    {
        auto board = Board("1k6/p6p/K6P/8/8/8/8/1q4q1 b - - 0 1");
        Move move = Move(B1, H7, KNIGHT, PAWN, EMPTY_PIECE, false, false, false);
        Board::StateBackup state = board.GetStateBackup();
        board.Make(move);
        board.Unmake(move, state);
        REQUIRE(board.GetFen() == "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1\n");
    }

    SECTION("castle rnbqkbnr/3ppppp/8/ppp5/6P1/5N1B/PPPPPP1P/RNBQK2R w KQkq - 0 4")
    {
        auto board = Board("rnbqkbnr/3ppppp/8/ppp5/6P1/5N1B/PPPPPP1P/RNBQK2R w KQkq - 0 4");
        Move move = Move(E1, G1, KING, EMPTY_PIECE, EMPTY_PIECE, false, false, true);
        Board::StateBackup state = board.GetStateBackup();
        board.Make(move);
        board.Unmake(move, state);
        REQUIRE(board.GetFen() == "rnbqkbnr/3ppppp/8/ppp5/6P1/5N1B/PPPPPP1P/RNBQK2R w KQkq - 0 4\n");
    }

    SECTION("pawn capture rnbqkbnr/3ppppp/8/ppp5/2P3P1/5N1B/PP1PPP1P/RNBQK2R b KQkq - 0 4")
    {
        auto board = Board("rnbqkbnr/3ppppp/8/ppp5/2P3P1/5N1B/PP1PPP1P/RNBQK2R b KQkq - 0 4");
        Move move = Move(B5, C4, PAWN, PAWN, EMPTY_PIECE, false, false, false);
        REQUIRE(move.is_capture() == true);
        Board::StateBackup state = board.GetStateBackup();
        board.Make(move);
        board.Unmake(move, state);
        REQUIRE(board.GetFen() == "rnbqkbnr/3ppppp/8/ppp5/2P3P1/5N1B/PP1PPP1P/RNBQK2R b KQkq - 0 4\n");
    }

    SECTION("promotion 4kbnr/P2ppppp/3q4/8/6P1/5N1B/PP1PPP1P/RNBQK1KR w k - 0 4")
    {
        auto board = Board("4kbnr/P2ppppp/3q4/8/6P1/5N1B/PP1PPP1P/RNBQK1KR w k - 0 4");
        Move move = Move(A7, A8, PAWN, EMPTY_PIECE, QUEEN, false, false, false);
        Board::StateBackup state = board.GetStateBackup();
        board.Make(move);
        board.Unmake(move, state);
        REQUIRE(board.GetFen() == "4kbnr/P2ppppp/3q4/8/6P1/5N1B/PP1PPP1P/RNBQK1KR w k - 0 4\n");
    }

    SECTION("en passant rnbqkbnr/3ppppp/8/p1p5/2pP2P1/5N1B/PP2PP1P/RNBQK2R b KQkq d3 0 5")
    {
        auto board = Board("rnbqkbnr/3ppppp/8/p1p5/2pP2P1/5N1B/PP2PP1P/RNBQK2R b KQkq d3 0 5");
        Move move = Move(C4, D3, PAWN, PAWN, EMPTY_PIECE, false, true, false);
        Board::StateBackup state = board.GetStateBackup();
        board.Make(move);
        board.Unmake(move, state);
        REQUIRE(board.GetFen() == "rnbqkbnr/3ppppp/8/p1p5/2pP2P1/5N1B/PP2PP1P/RNBQK2R b KQkq d3 0 5\n");
    }

    SECTION("d2d4 rnbqkbnr/pppppppp/8/8/3P4/8/PPP1PPPP/RNBQKBNR w KQkq - 0 1")
    {
        auto board = Board("rnbqkbnr/pppppppp/8/8/3P4/8/PPP1PPPP/RNBQKBNR w KQkq - 0 1");
        Move move = Move(D2, D4, PAWN, EMPTY_PIECE, EMPTY_PIECE, true, false, false);
        Board::StateBackup state = board.GetStateBackup();
        board.Make(move);
        board.Unmake(move, state);
        REQUIRE(board.GetFen() == "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1\n");
    }
}
