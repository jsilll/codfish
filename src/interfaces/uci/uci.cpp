#include <interfaces/uci/uci.hpp>

#include <interfaces/utils.hpp>

#include <engine/movegen/magics.hpp>
#include <engine/movegen/tables.hpp>
#include <engine/board.hpp>

#include <engine/movepicker/eval.hpp>
#include <engine/zobrist.hpp>

#include <interfaces/uci/commands/commands.hpp>

#include <iostream>
#include <string>
#include <vector>

namespace uci
{
    void init()
    {
        magics::init();
        tables::init();
        eval::init();
        zobrist::init();

        Board board = Board();

        // TODO: use a map instead of if statements
        uci::UCICommand uciCommand = UCICommand(board);
        uci::IsReadyCommand isReadyCommand = IsReadyCommand(board);
        uci::UCINewGameCommand uciNewGameCommand = UCINewGameCommand(board);
        uci::PositionCommand positionCommand = PositionCommand(board);
        uci::DisplayCommand displayCommand = DisplayCommand(board);
        uci::GoCommand goCommand = GoCommand(board);
        uci::QuitCommand quitCommand = QuitCommand(board);

        std::string line;
        while (std::getline(std::cin, line))
        {
            std::string cmd;
            std::vector<std::string> args = utils::tokenize(line);
            if (!args.empty())
            {
                cmd = args[0];
                args.erase(args.begin());
            }
            else
            {
                continue;
            }

            if (cmd == "uci")
            {
                uciCommand.execute(args);
            }
            else if (cmd == "isready")
            {
                isReadyCommand.execute(args);
            }
            else if (cmd == "ucinewgame")
            {
                uciNewGameCommand.execute(args);
            }
            else if (cmd == "position")
            {
                positionCommand.execute(args);
            }
            else if (cmd == "d")
            {
                displayCommand.execute(args);
            }
            else if (cmd == "go")
            {
                goCommand.execute(args);
            }
            else if (cmd == "quit")
            {
                quitCommand.execute(args);
            }
        }
    }

} // namespace uci
