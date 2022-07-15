#include <interfaces/uci/uci.hpp>

#include <interfaces/utils.hpp>

#include <engine/defs.hpp>

#include <engine/movegen/magics.hpp>
#include <engine/movegen/tables.hpp>
#include <engine/movegen/board.hpp>
#include <engine/movegen/move.hpp>
#include <engine/movegen/movegen.hpp>

#include <engine/movepicker/movepicker.hpp>
#include <engine/movepicker/eval.hpp>

#include <interfaces/uci/commands/display.hpp>
#include <interfaces/uci/commands/exit.hpp>
#include <interfaces/uci/commands/go.hpp>
#include <interfaces/uci/commands/isready.hpp>
#include <interfaces/uci/commands/position.hpp>
#include <interfaces/uci/commands/uci.hpp>
#include <interfaces/uci/commands/ucinewgame.hpp>

#include <chrono>
#include <climits>
#include <iostream>
#include <string>
#include <vector>

namespace interfaces::uci
{
    void init()
    {
        magics::init();
        tables::init();
        eval::init();

        Board board = Board();

        interfaces::uci::commands::UCICommand uciCommand;
        interfaces::uci::commands::IsReadyCommand isReadyCommand;
        interfaces::uci::commands::UCINewGameCommand uciNewGameCommand;
        interfaces::uci::commands::PositionCommand positionCommand;
        interfaces::uci::commands::DisplayCommand displayCommand;
        interfaces::uci::commands::GoCommand goCommand;
        interfaces::uci::commands::QuitCommand quitCommand;

        std::string line;
        while (std::getline(std::cin, line))
        {
            std::vector<std::string> args = interfaces::utils::tokenizeString(line);
            std::string cmd = args[0];
            args.erase(args.begin());
            if (cmd == "uci")
            {
                uciCommand.execute(args, board);
            }
            else if (cmd == "isready")
            {
                isReadyCommand.execute(args, board);
            }
            else if (cmd == "ucinewgame")
            {
                uciNewGameCommand.execute(args, board);
            }
            else if (cmd == "position")
            {
                positionCommand.execute(args, board);
            }
            else if (cmd == "d")
            {
                displayCommand.execute(args, board);
            }
            else if (cmd == "go")
            {
                goCommand.execute(args, board);
            }
            else if (cmd == "quit")
            {
                quitCommand.execute(args, board);
            }
        }
    }

} // namespace interfaces::uci
