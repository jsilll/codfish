#define CATCH_CONFIG_MAIN

#include "../catch.hpp"

#include <engine/defs.hpp>
#include <engine/movegen/magics.hpp>
#include <engine/movegen/tables.hpp>
#include <engine/movegen/board.hpp>
#include <engine/movegen/move.hpp>
#include <engine/movepicker/movepicker.hpp>

void setup()
{
    magics::init();
    tables::init();
}

TEST_CASE("unmake move")
{
    setup();

    Board board = Board();

    SECTION("PAWNS rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1")
    {
        // board.setFromFen("1k6/p6p/K6P/8/8/8/8/1q4q1", "b", "-", "-", "0", "1");
        Move move = Move(E2, E4, PAWN, EMPTY_PIECE, EMPTY_PIECE, true, false, false);
        Board::info board_info = board.getBoardInfo();
        board.makeMove(move);
        board.unmakeMove(move, board_info);
        REQUIRE(board.getFen() == "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1\n");
    }

    SECTION("PAWNS rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1")
    {
        // board.setFromFen("1k6/p6p/K6P/8/8/8/8/1q4q1", "b", "-", "-", "0", "1");
        Move move = Move(B1, H7, KNIGHT, PAWN, EMPTY_PIECE, false, false, false);
        Board::info board_info = board.getBoardInfo();
        board.makeMove(move);
        board.unmakeMove(move, board_info);
        REQUIRE(board.getFen() == "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1\n");
    }
}
