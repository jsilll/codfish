#include <interfaces/cli/commands/commands.hpp>

#include <engine/bitboard.hpp>

#include <iostream>

void cli::InfoCommand::execute([[maybe_unused]] std::vector<std::string> &args)
{
    std::string fen = _board.get_fen();
    std::vector<std::string> split_fen = utils::tokenize(fen);
    std::cout << "Side to Play                 = " << split_fen[1]
              << "\nCastling Rights              = " << split_fen[2]
              << "\nEn-passant Square            = " << split_fen[3]
              << "\nFifty Move Count             = " << split_fen[4]
              << "\nFull Move Number             = " << split_fen[5];
    std::cout << "\nOccupied Squares:\n";

    std::cout << "WHITE, PAWN" << std::endl;
    bitboard::print(_board.get_pieces(WHITE, PAWN));
    std::cout << "WHITE, KNIGHT" << std::endl;
    bitboard::print(_board.get_pieces(WHITE, KNIGHT));
    std::cout << "WHITE, BISHOP" << std::endl;
    bitboard::print(_board.get_pieces(WHITE, BISHOP));
    std::cout << "WHITE, ROOK" << std::endl;
    bitboard::print(_board.get_pieces(WHITE, ROOK));
    std::cout << "WHITE, QUEEN" << std::endl;
    bitboard::print(_board.get_pieces(WHITE, QUEEN));
    std::cout << "WHITE, KING" << std::endl;
    bitboard::print(_board.get_pieces(WHITE, KING));
    std::cout << "BLACK, PAWN" << std::endl;
    bitboard::print(_board.get_pieces(BLACK, PAWN));
    std::cout << "BLACK, KNIGHT" << std::endl;
    bitboard::print(_board.get_pieces(BLACK, KNIGHT));
    std::cout << "BLACK, BISHOP" << std::endl;
    bitboard::print(_board.get_pieces(BLACK, BISHOP));
    std::cout << "BLACK, ROOK" << std::endl;
    bitboard::print(_board.get_pieces(BLACK, ROOK));
    std::cout << "BLACK, QUEEN" << std::endl;
    bitboard::print(_board.get_pieces(BLACK, QUEEN));
    std::cout << "BLACK, KING" << std::endl;
    bitboard::print(_board.get_pieces(BLACK, KING));

    std::cout << "BOTH" << std::endl;
    bitboard::print(_board.get_occupancies(BOTH));
    std::cout << "WHITE" << std::endl;
    bitboard::print(_board.get_occupancies(WHITE));
    std::cout << "BLACK" << std::endl;
    bitboard::print(_board.get_occupancies(BLACK));
}