#include "cli.hpp"

#include <iostream>
#include <string>
#include <vector>

#include "../utils.hpp"
#include "commands/commands.hpp"

#include <codchess/board.hpp>
#include <codchess/move.hpp>

using namespace codchess;

namespace cli
{
  void init()
  {
    ::codchess::Init();
    Board board = Board();

    AsciiCommand ascii_command = AsciiCommand(board);
    CapturesCommand captures_command = CapturesCommand(board);
    DisplayCommand display_command = DisplayCommand(board);
    DividedPerftCommand dperft_command = DividedPerftCommand(board);
    EvalCommand eval_command = EvalCommand(board);
    ExitCommand exit_command = ExitCommand(board);
    GetFenCommand get_fen_command = GetFenCommand(board);
    HelpCommand help_command = HelpCommand(board);
    InfoCommand info_command = InfoCommand(board);
    MagicsCommand magics_command = MagicsCommand(board);
    MoveCommand move_command = MoveCommand(board);
    MovesCommand moves_command = MovesCommand(board);
    NewCommand new_command = NewCommand(board);
    PerftCommand perft_command = PerftCommand(board);
    PLMovesCommand pl_moves_command = PLMovesCommand(board);
    RotateCommand rotate_command = RotateCommand(board);
    SetFenCommand set_fen_command = SetFenCommand(board);
    SwitchCommand switch_command = SwitchCommand(board);

    for (;;)
    {
      std::cout << "> " << std::flush;

      std::string line;
      std::getline(std::cin, line);
      std::vector<std::string> args = utils::tokenize(std::string(line));

      if (args.empty())
      {
        continue;
      }

      std::string cmd = args[0];
      args.erase(args.begin());

      if (cmd == "help")
      {
        help_command.execute(args);
      }
      else if (cmd == "ascii")
      {
        ascii_command.execute(args);
      }
      else if (cmd == "display")
      {
        display_command.execute(args);
      }
      else if (cmd == "dperft")
      {
        dperft_command.execute(args);
      }
      else if (cmd == "eval")
      {
        eval_command.execute(args);
      }
      else if (cmd == "exit")
      {
        exit_command.execute(args);
      }
      else if (cmd == "info")
      {
        info_command.execute(args);
      }
      else if (cmd == "magics")
      {
        magics_command.execute(args);
      }
      else if (cmd == "move")
      {
        move_command.execute(args);
      }
      else if (cmd == "moves")
      {
        moves_command.execute(args);
      }
      else if (cmd == "plmoves")
      {
        pl_moves_command.execute(args);
      }
      else if (cmd == "captures")
      {
        captures_command.execute(args);
      }
      else if (cmd == "new")
      {
        new_command.execute(args);
      }
      else if (cmd == "perft")
      {
        perft_command.execute(args);
      }
      else if (cmd == "getfen")
      {
        get_fen_command.execute(args);
      }
      else if (cmd == "rotate")
      {
        rotate_command.execute(args);
      }
      else if (cmd == "setfen")
      {
        set_fen_command.execute(args);
      }
      else if (cmd == "switch")
      {
        switch_command.execute(args);
      }
      else
      {
        std::cout << "command not found" << std::endl;
      }
    }
  }

} // namespace cli