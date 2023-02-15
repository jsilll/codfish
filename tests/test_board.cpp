#include <catch2/catch_test_macros.hpp>

#include <codlib/board.hpp>

TEST_CASE("Board::Board() sets the board correctly", "[board]")
{
  SECTION("Position 1")
  {
    const auto board = Board();

    REQUIRE(board.get_half_move_clock() == 0);
    REQUIRE(board.get_full_move_number() == 1);
    REQUIRE(board.get_side_to_move() == Color::WHITE);
    REQUIRE(board.get_en_passant_square() == Square::EMPTY_SQUARE);
    REQUIRE(board.get_castling_rights() == CastlingRight::CASTLE_ALL);

    REQUIRE(board.get_piece_from_square(Square::A1) == Board::Piece{ PieceType::ROOK, Color::WHITE });
    REQUIRE(board.get_piece_from_square(Square::A2) == Board::Piece{ PieceType::PAWN, Color::WHITE });
    REQUIRE(board.get_piece_from_square(Square::A3) == Board::Piece{ PieceType::EMPTY_PIECE, Color::BOTH });

    REQUIRE(board.get_piece_from_square(Square::H1) == Board::Piece{ PieceType::ROOK, Color::WHITE });
    REQUIRE(board.get_piece_from_square(Square::H2) == Board::Piece{ PieceType::PAWN, Color::WHITE });
    REQUIRE(board.get_piece_from_square(Square::H3) == Board::Piece{ PieceType::EMPTY_PIECE, Color::BOTH });

    REQUIRE(board.get_piece_from_square(Square::A8) == Board::Piece{ PieceType::ROOK, Color::BLACK });
    REQUIRE(board.get_piece_from_square(Square::A7) == Board::Piece{ PieceType::PAWN, Color::BLACK });
    REQUIRE(board.get_piece_from_square(Square::A6) == Board::Piece{ PieceType::EMPTY_PIECE, Color::BOTH });

    REQUIRE(board.get_piece_from_square(Square::H8) == Board::Piece{ PieceType::ROOK, Color::BLACK });
    REQUIRE(board.get_piece_from_square(Square::H7) == Board::Piece{ PieceType::PAWN, Color::BLACK });
    REQUIRE(board.get_piece_from_square(Square::H6) == Board::Piece{ PieceType::EMPTY_PIECE, Color::BOTH });

    REQUIRE(board.get_piece_from_square(Square::D3) == Board::Piece{ PieceType::EMPTY_PIECE, Color::BOTH });
    REQUIRE(board.get_piece_from_square(Square::D4) == Board::Piece{ PieceType::EMPTY_PIECE, Color::BOTH });
    REQUIRE(board.get_piece_from_square(Square::D5) == Board::Piece{ PieceType::EMPTY_PIECE, Color::BOTH });
    REQUIRE(board.get_piece_from_square(Square::D6) == Board::Piece{ PieceType::EMPTY_PIECE, Color::BOTH });

    REQUIRE(board.get_fen() == "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1");
  }
}

TEST_CASE("Board::set_from_fen() sets the board correctly", "[board]")
{
  SECTION("Position 1")
  {
    const auto board = Board("r1b1kb1r/ppp1p2p/5pp1/7K/4pPn1/8/PP1P2nP/RNB2qNR", "w", "kq", "-", "0", "14");

    REQUIRE(board.get_half_move_clock() == 0);
    REQUIRE(board.get_full_move_number() == 14);
    REQUIRE(board.get_side_to_move() == Color::WHITE);
    REQUIRE(board.get_en_passant_square() == Square::EMPTY_SQUARE);
    REQUIRE(board.get_castling_rights() == (CastlingRight::CASTLE_KING_BLACK | CastlingRight::CASTLE_QUEEN_BLACK));

    REQUIRE(board.get_piece_from_square(Square::A1) == Board::Piece{ PieceType::ROOK, Color::WHITE });
    REQUIRE(board.get_piece_from_square(Square::A2) == Board::Piece{ PieceType::PAWN, Color::WHITE });

    REQUIRE(board.get_fen() == "r1b1kb1r/ppp1p2p/5pp1/7K/4pPn1/8/PP1P2nP/RNB2qNR w kq - 0 14");
  }
}

TEST_CASE("Board::get_fen() returns the correct FEN", "[board]")
{
  SECTION("Position 1")
  {
    const auto board = Board("r1b1kb1r/ppp1p2p/5pp1/7K/4pPn1/8/PP1P2nP/RNB2qNR", "w", "kq", "-", "0", "14");

    REQUIRE(board.get_fen() == "r1b1kb1r/ppp1p2p/5pp1/7K/4pPn1/8/PP1P2nP/RNB2qNR w kq - 0 14");
  }
}