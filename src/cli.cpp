#include "cli.hpp"

#include "defs.hpp"
#include "utils.hpp"
#include "bitboard.hpp"
#include "magics.hpp"
#include "tables.hpp"
#include "board.hpp"
#include "move.hpp"
#include "movelist.hpp"
#include "movegen.hpp"
#include "perft.hpp"
#include "eval.hpp"

#include <algorithm>
#include <chrono>
#include <iostream>
#include <regex>
#include <string>
#include <vector>

namespace cli
{
  /**
   * @brief Command Abstract class
   * All commands should implement this class
   *
   */
  class Command
  {
  public:
    virtual void execute(std::vector<std::string> &args, Board &board) = 0;
  };

  /**
   * @brief Handles the 'dperft' command
   *
   */
  class DividedPerftCommand : public Command
  {
  public:
    void execute(std::vector<std::string> &args, Board &board)
    {
      if (args.size() == 0)
      {
        std::cout << "dperft command takes exactly one argument" << std::endl;
        return;
      }

      int depth = 0;
      try
      {
        depth = std::stoi(args[0]);
      }
      catch (const std::exception &e)
      {
        std::cout << "invalid depth value1." << std::endl;
        return;
      }

      if (depth >= 0)
      {
        this->perft(depth, board);
      }
      else
      {
        std::cout << "invalid depth value." << std::endl;
      }
    }

  private:
    void perft(int depth, Board &board)
    {
      unsigned long long total_nodes = 0;
      std::chrono::time_point<std::chrono::system_clock> start = std::chrono::system_clock::now();
      for (Move const &move : movegen::generatePseudoLegalMoves(board))
      {
        Board backup = board;
        backup.makeMove(move);
        int king_sq = bitboard::bitScanForward(backup.getPieces(backup.getOpponent(), KING));
        int attacker_side = backup.getSideToMove();
        if (!backup.isSquareAttacked(king_sq, attacker_side))
        {
          unsigned long long nodes = perft::perft(backup, depth - 1);
          std::cout << move.getUCI() << ": " << nodes << std::endl;
          total_nodes += nodes;
        }
      }
      std::chrono::time_point<std::chrono::system_clock> end = std::chrono::system_clock::now();
      std::chrono::duration<double> elapsed_seconds = end - start;
      std::time_t end_time = std::chrono::system_clock::to_time_t(end);
      std::cout << "Found " << total_nodes << " nodes." << std::endl;
      std::cout << "Finished computation at " << std::ctime(&end_time);
      std::cout << "Elapsed time: " << elapsed_seconds.count() << "s" << std::endl;
      std::cout << "Nodes Per Second: " << (double)total_nodes / elapsed_seconds.count() << std::endl;
    }
  } dperftCommand;

  /**
   * @brief Handles the 'help' command
   *
   */
  class HelpCommand : public Command
  {
  public:
    void execute([[maybe_unused]] std::vector<std::string> &args, [[maybe_unused]] Board &board)
    {
      std::cout
          << "ascii                 Toggles between ascii and utf-8 board representation\n"
          << "cc                    Plays computer-to-computer [TODO]\n"
          << "display               Displays board \n"
          << "dperft (n)            Divided perft\n"
          << "eval                  Shows static evaluation of this position\n"
          << "exit                  Exits program\n"
          << "go                    Computer plays his best move [TODO]\n"
          << "help                  Shows this help \n"
          << "info                  Displays variables (for testing purposes)\n"
          << "magics                Generates magic numbers for the bishop and rook pieces\n"
          << "move (move)           Plays a move (in uci format)\n"
          << "moves                 Shows all legal moves\n"
          << "new                   Starts new game\n"
          << "perf                  Benchmarks a number of key functions [TODO]\n"
          << "perft n               Calculates raw number of nodes from here, depth n\n"
          << "getfen                Prints current position to in fen string format \n"
          << "rotate                Rotates board \n"
          << "setfen (fen)          Reads fen string position and modifies board acoordingly\n"
          << "sd (n)                Sets the search depth to n [TODO]\n"
          << "switch                Switches the next side to move\n"
          << "undo                  Takes back last move [TODO]\n"
          << std::endl;
    }
  } helpCommand;

  /**
   * @brief Handles the 'info' command
   *
   */
  class InfoCommand : public Command
  {
  public:
    void execute([[maybe_unused]] std::vector<std::string> &args, Board &board)
    {
      std::string fen = board.getFen();
      std::vector<std::string> splitted_fen = utils::tokenizeString(fen);
      std::cout << "Side to Play                 = " << splitted_fen[1]
                << "\nCastling Rights              = " << splitted_fen[2]
                << "\nEn-passant Square            = " << splitted_fen[3]
                << "\nFifty Move Count             = " << splitted_fen[4]
                << "\nFull Move Number             = " << splitted_fen[5];
      std::cout << "\nOccupied Squares:\n";
      bitboard::printBB(board.getOccupancies(BOTH));
      bitboard::printBB(board.getOccupancies(WHITE));
      bitboard::printBB(board.getOccupancies(BLACK));
    }
  } infoCommand;

  /**
   * @brief Handles the 'move' command
   *
   */
  class MoveCommand : public Command
  {
  public:
    void execute(std::vector<std::string> &args, Board &board)
    {
      if (args.size() == 0)
      {
        return;
      }

      for (Move const &move : movegen::generateLegalMoves(board))
      {
        if (move.getUCI() == args[0])
        {
          board.makeMove(move);
          return;
        }
      }

      std::cout << "invalid move" << std::endl;
    }
  } moveCommand;

  /**
   * @brief Handles 'moves' command
   *
   */
  class MovesCommand : public Command
  {
  public:
    void execute([[maybe_unused]] std::vector<std::string> &args, Board &board)
    {
      MoveList moves = movegen::generateLegalMoves(board);
      for (Move const &move : moves)
      {
        std::cout << move.getUCI() << "\n";
      }
      std::cout << "Total number of moves: " << moves.size() << std::endl;
    }
  } movesCommand;

  /**
   * @brief Handles the 'perft' command
   *
   */
  class PerftCommand : public Command
  {
  public:
    void execute(std::vector<std::string> &args, Board &board)
    {

      if (args.size() == 0)
      {
        std::cout << "perft command takes exactly one argument" << std::endl;
        return;
      }

      int depth = 0;
      try
      {
        depth = std::stoi(args[0]);
      }
      catch (const std::exception &e)
      {
        std::cout << "invalid depth value." << std::endl;
        return;
      }
      if (depth >= 0)
      {
        this->dperft(depth, board);
      }
      else
      {
        std::cout << "invalid depth value." << std::endl;
      }
    }

  private:
    void dperft(int depth, Board board)
    {
      std::chrono::time_point<std::chrono::system_clock> start = std::chrono::system_clock::now();
      unsigned long long nodes = perft::perft(board, depth);
      std::chrono::time_point<std::chrono::system_clock> end = std::chrono::system_clock::now();
      std::chrono::duration<double> elapsed_seconds = end - start;
      std::time_t end_time = std::chrono::system_clock::to_time_t(end);
      std::cout << "Found " << nodes << " nodes." << std::endl;
      std::cout << "Finished computation at " << std::ctime(&end_time);
      std::cout << "Elapsed time: " << elapsed_seconds.count() << "s" << std::endl;
      std::cout << "Nodes Per Second: " << (double)nodes / elapsed_seconds.count() << std::endl;
    }
  } perftCommand;

  /**
   * @brief Handles the 'switch' command
   *
   */
  class SwitchCommand : public Command
  {
  public:
    void execute([[maybe_unused]] std::vector<std::string> &args, Board &board)
    {
      std::cout << "side to play is now " << (board.switchSideToMove() == WHITE ? "white" : "black") << std::endl;
    }
  } switchCommand;

  /**
   * @brief Executes the 'display' command
   *
   */
  class DisplayCommand : public Command
  {
  public:
    void execute([[maybe_unused]] std::vector<std::string> &args, Board &board)
    {
      board.display();
    }
  } displayCommand;

  /**
   * @brief Handles the 'new' command
   *
   */
  class NewCommand : public Command
  {
  public:
    void execute([[maybe_unused]] std::vector<std::string> &args, Board &board)
    {
      board.setStartingPosition();
    }
  } newCommand;

  /**
   * @brief Handles the 'rotate' command
   *
   */
  class RotateCommand : public Command
  {
  public:
    void execute([[maybe_unused]] std::vector<std::string> &args, Board &board)
    {
      std::cout << (board.rotateDisplay() ? "white" : "black") << " is now on bottom" << std::endl;
    }
  } rotateCommand;

  /**
   * @brief Handles the 'setfen' command
   *
   */
  class SetFenCommand : public Command
  {
  public:
    void execute(std::vector<std::string> &args, Board &board)
    {
      if (!this->isFenValid(args))
      {
        std::cout << "Invalid FEN String" << std::endl;
        return;
      }

      board.setFromFen(args[1], args[2], args[3], args[4], args[5], args[6]);
    }

  private:
    bool isFenValid(std::vector<std::string> &args)
    {
      if (args.size() != 7)
      {
        return false;
      }

      static const std::regex piece_placements_regex(R"((([pnbrqkPNBRQK1-8]{1,8})\/?){8})");
      static const std::regex active_color_regex(R"(b|w)");
      static const std::regex castling_rights_regex(R"(-|K?Q?k?q?)");
      static const std::regex en_passant_regex(R"(-|[a-h][3-6])");
      static const std::regex halfmove_clock_regex(R"(\d+)");
      static const std::regex fullmove_number_regex(R"(\d+)");
      if (!std::regex_match(args[1], piece_placements_regex) ||
          !std::regex_match(args[2], active_color_regex) ||
          !std::regex_match(args[3], castling_rights_regex) ||
          !std::regex_match(args[4], en_passant_regex) ||
          !std::regex_match(args[5], halfmove_clock_regex) ||
          !std::regex_match(args[6], fullmove_number_regex))
      {
        return false;
      }

      return true;
    }
  } setFenCommand;

  /**
   * @brief Handles the 'getfen' command
   *
   */
  class GetFenCommand : public Command
  {
  public:
    void execute([[maybe_unused]] std::vector<std::string> &args, Board &board)
    {
      std::cout << board.getFen();
    }
  } getFenCommand;

  /**
   * @brief Handles the 'magics' command
   *
   */
  class MagicsCommand : public Command
  {
  public:
    void execute([[maybe_unused]] std::vector<std::string> &args, [[maybe_unused]] Board &board)
    {
      magics::generate();
    }
  } magicsCommand;

  /**
   * @brief Handles the 'ascii' command
   *
   */
  class AsciiCommand : public Command
  {
  public:
    void execute([[maybe_unused]] std::vector<std::string> &args, Board &board)
    {
      std::cout << "ascii mode toggled " << (board.toggleAscii() ? "on" : "off") << std::endl;
    }
  } asciiCommand;

  /**
   * @brief Handles the 'eval' command
   *
   */
  class EvalCommand : public Command
  {
  public:
    void execute([[maybe_unused]] std::vector<std::string> &args, Board &board)
    {
      std::cout << "Static Evaluation: " << eval::eval(board) << std::endl;
    }
  } evalCommand;

  /**
   * @brief Handles the 'exit' command
   *
   */
  class ExitCommand : public Command
  {
  public:
    void execute([[maybe_unused]] std::vector<std::string> &args, [[maybe_unused]] Board &board)
    {
      exit(EXIT_SUCCESS);
    }
  } exitCommand;

  void init()
  {
    magics::init();
    tables::init();
    eval::init();

    Board board = Board();

    while (true)
    {
      std::cout << "> ";
      std::string line;
      std::getline(std::cin, line);
      std::vector<std::string> args = utils::tokenizeString(std::string(line));
      std::string cmd = args[0];
      args.erase(args.begin());
      if (cmd == "help")
      {
        helpCommand.execute(args, board);
      }
      else if (cmd == "ascii")
      {
        asciiCommand.execute(args, board);
      }
      else if (cmd == "display")
      {
        displayCommand.execute(args, board);
      }
      else if (cmd == "dperft")
      {
        dperftCommand.execute(args, board);
      }
      else if (cmd == "eval")
      {
        evalCommand.execute(args, board);
      }
      else if (cmd == "exit")
      {
        exitCommand.execute(args, board);
      }
      else if (cmd == "help")
      {
        helpCommand.execute(args, board);
      }
      else if (cmd == "info")
      {
        infoCommand.execute(args, board);
      }
      else if (cmd == "magics")
      {
        magicsCommand.execute(args, board);
      }
      else if (cmd == "move")
      {
        moveCommand.execute(args, board);
      }
      else if (cmd == "moves")
      {
        movesCommand.execute(args, board);
      }
      else if (cmd == "new")
      {
        newCommand.execute(args, board);
      }
      else if (cmd == "perft")
      {
        perftCommand.execute(args, board);
      }
      else if (cmd == "getfen")
      {
        getFenCommand.execute(args, board);
      }
      else if (cmd == "rotate")
      {
        rotateCommand.execute(args, board);
      }
      else if (cmd == "setfen")
      {
        setFenCommand.execute(args, board);
      }
      else if (cmd == "switch")
      {
        switchCommand.execute(args, board);
      }
      else
      {
        std::cout << "command not found" << std::endl;
      }
    }
  }
} // namespace cli