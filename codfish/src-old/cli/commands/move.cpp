#include "commands.hpp"

#include <iostream>

void cli::MoveCommand::execute(std::vector<std::string> &args)
{
    std::cout << "TODO\n";    

    if (args.size() == 0)
    {
        return;
    }

    // for (const auto move : movegen::generateLegalMoves(_board))
    // {
    //     if (move.get_uci() == args[0])
    //     {
    //         _board.Make(move);
    //         return;
    //     }
    // }

    // std::cout << "invalid move" << std::endl;
}