#include <interfaces/cli/commands/commands.hpp>

#include <regex>
#include <iostream>

void cli::SetFenCommand::execute(std::vector<std::string> &args, Board &board)
{
    if (!utils::is_fen_valid(args))
    {
        std::cout << "Invalid FEN String" << std::endl;
        return;
    }

    board.set_from_fen(args[0], args[1], args[2], args[3], args[4], args[5]);
}
