#include <interfaces/cli/cli.hpp>

#include <interfaces/utils.hpp>

#include <interfaces/cli/commands/commands.hpp>

#include <engine/board.hpp>
#include <engine/move.hpp>

#include <engine/movegen/magics.hpp>
#include <engine/movegen/tables.hpp>

#include <engine/movepicker/eval.hpp>

#include <iostream>
#include <string>
#include <vector>

namespace cli
{
  void init()
  {
    magics::init();
    tables::init();
    eval::init();

    AsciiCommand ascii_command = AsciiCommand();
    CapturesCommand captures_command = CapturesCommand();
    DisplayCommand display_command = DisplayCommand();
    DividedPerftCommand dperft_command = DividedPerftCommand();
    EvalCommand eval_command = EvalCommand();
    ExitCommand exit_command = ExitCommand();
    GetFenCommand get_fen_command = GetFenCommand();
    HelpCommand help_command = HelpCommand();
    InfoCommand info_command = InfoCommand();
    MagicsCommand magics_command = MagicsCommand();
    MoveCommand move_command = MoveCommand();
    MovesCommand moves_command = MovesCommand();
    NewCommand new_command = NewCommand();
    PerftCommand perft_command = PerftCommand();
    PLMovesCommand pl_moves_command = PLMovesCommand();
    RotateCommand rotate_command = RotateCommand();
    SetFenCommand set_fen_command = SetFenCommand();
    SwitchCommand switch_command = SwitchCommand();

    Board board = Board();

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
        help_command.execute(args, board);
      }
      else if (cmd == "ascii")
      {
        ascii_command.execute(args, board);
      }
      else if (cmd == "display")
      {
        display_command.execute(args, board);
      }
      else if (cmd == "dperft")
      {
        dperft_command.execute(args, board);
      }
      else if (cmd == "eval")
      {
        eval_command.execute(args, board);
      }
      else if (cmd == "exit")
      {
        exit_command.execute(args, board);
      }
      else if (cmd == "info")
      {
        info_command.execute(args, board);
      }
      else if (cmd == "magics")
      {
        magics_command.execute(args, board);
      }
      else if (cmd == "move")
      {
        move_command.execute(args, board);
      }
      else if (cmd == "moves")
      {
        moves_command.execute(args, board);
      }
      else if (cmd == "plmoves")
      {
        pl_moves_command.execute(args, board);
      }
      else if (cmd == "captures")
      {
        captures_command.execute(args, board);
      }
      else if (cmd == "new")
      {
        new_command.execute(args, board);
      }
      else if (cmd == "perft")
      {
        perft_command.execute(args, board);
      }
      else if (cmd == "getfen")
      {
        get_fen_command.execute(args, board);
      }
      else if (cmd == "rotate")
      {
        rotate_command.execute(args, board);
      }
      else if (cmd == "setfen")
      {
        set_fen_command.execute(args, board);
      }
      else if (cmd == "switch")
      {
        switch_command.execute(args, board);
      }
      else
      {
        std::cout << "command not found" << std::endl;
      }
    }
  }

} // namespace cli