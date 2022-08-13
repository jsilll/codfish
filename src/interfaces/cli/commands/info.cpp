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
    bitboard::print(_board.get_occupancies(BOTH));
    bitboard::print(_board.get_occupancies(WHITE));
    bitboard::print(_board.get_occupancies(BLACK));
}