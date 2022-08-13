#include <interfaces/cli/commands/commands.hpp>

#include <engine/movegen/movegen.hpp>

#include <iostream>

void cli::MoveCommand::execute(std::vector<std::string> &args)
{
    if (args.size() == 0)
    {
        return;
    }

    for (const Move &move : movegen::generateLegalMoves(_board))
    {
        if (move.get_uci() == args[0])
        {
            _board.make_move(move);
            return;
        }
    }

    std::cout << "invalid move" << std::endl;
}