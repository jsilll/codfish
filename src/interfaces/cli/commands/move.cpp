#include <interfaces/cli/commands/commands.hpp>

#include <engine/movegen/movegen.hpp>

#include <iostream>

void cli::MoveCommand::execute(std::vector<std::string> &args, Board &board)
{
    if (args.size() == 0)
    {
        return;
    }

    for (const Move &move : movegen::generateLegalMoves(board))
    {
        if (move.get_uci() == args[0])
        {
            board.make_move(move);
            return;
        }
    }

    std::cout << "invalid move" << std::endl;
}