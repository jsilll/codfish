#include "cli.hpp"

#include <algorithm>
#include <chrono>
#include <iostream>
#include <regex>
#include <string>
#include <vector>

#include <readline/readline.h>
#include <readline/history.h>

#include "defs.hpp"
#include "utils.hpp"
#include "magics.hpp"
#include "tables.hpp"
#include "board.hpp"
#include "move.hpp"
#include "movelist.hpp"
#include "movegen.hpp"
#include "perft.hpp"
#include "ai.hpp"
#include "eval.hpp"

void dperftCommand(Board &board, int depth);
void helpCommand();
void infoCommand(const Board &board);
void moveCommand(Board &board, std::string move_uci);
void movesCommand(Board &board);
void parseCommand(std::string buf, Board &board);
void perftCommand(Board &board, int depth);

bool ASCII{};

void Cli::init()
{
  Magics::init();
  Tables::init();

  Board board = Board();

  char *cmd;
  while ((cmd = readline("> ")) != nullptr)
  {
    if (*cmd)
    {
      add_history(cmd);
      parseCommand(std::string(cmd), board);
      free(cmd);
    }
  }
}

void parseCommand(std::string buf, Board &board)
{
  std::vector<std::string> words = Utils::tokenizeString(buf);
  if (words.size() == 0)
  {
  }
  else if (words[0] == "help" || words[0] == "h" || words[0] == "?")
  {
    helpCommand();
  }
  else if (words[0] == "switch" || words[0] == "s")
  {
    std::cout << "side to play is now "
              << (board.switchSideToMove() == WHITE ? "white" : "black")
              << std::endl;
  }
  else if (words[0] == "display" || words[0] == "d")
  {
    board.display(ASCII);
  }
  else if (words[0] == "info" || words[0] == "i")
  {
    infoCommand(board);
  }
  else if (words[0] == "new" || words[0] == "n")
  {
    board.setStartingPosition();
  }
  else if (words[0] == "rotate" || words[0] == "r")
  {
    std::cout << (board.rotateDisplay() ? "white" : "black") << " is now on bottom"
              << std::endl;
  }
  else if (words[0] == "setfen")
  {
    static const std::regex piece_placements_regex(R"((([pnbrqkPNBRQK1-8]{1,8})\/?){8})");
    static const std::regex active_color_regex(R"(b|w)");
    static const std::regex castling_rights_regex(R"(-|K?Q?k?q?)");
    static const std::regex en_passant_regex(R"(-|[a-h][3-6])");
    static const std::regex halfmove_clock_regex(R"(\d+)");
    static const std::regex fullmove_number_regex(R"(\d+)");
    if (words.size() != 7 ||
        !std::regex_match(words[1], piece_placements_regex) ||
        !std::regex_match(words[2], active_color_regex) ||
        !std::regex_match(words[3], castling_rights_regex) ||
        !std::regex_match(words[4], en_passant_regex) ||
        !std::regex_match(words[5], halfmove_clock_regex) ||
        !std::regex_match(words[6], fullmove_number_regex))
    {
      std::cerr << "Invalid FEN String" << std::endl;
    }
    else
    {
      board.setFromFen(words[1], words[2], words[3], words[4], words[5], words[6]);
    }
  }
  else if (words[0] == "getfen")
  {
    std::cout << board.getFen();
  }
  else if (words[0] == "magics")
  {
    Magics::generate();
  }
  else if (words[0] == "move")
  {
    moveCommand(board, words[1]);
  }
  else if (words[0] == "moves")
  {
    movesCommand(board);
  }
  else if (words[0] == "ascii")
  {
    std::cout << "ascii mode toggled " << ((ASCII = !ASCII) ? "on" : "off")
              << std::endl;
  }
  else if (words[0] == "exit")
  {
    exit(EXIT_SUCCESS);
  }
  else if (words[0] == "perft")
  {
    if (words.size() == 2)
    {
      int depth{};
      try
      {
        depth = std::stoi(words[1]);
      }
      catch (const std::exception &e)
      {
        std::cout << "invalid depth value." << std::endl;
        return;
      }
      if (depth >= 0)
      {
        perftCommand(board, depth);
      }
      else
      {
        std::cout << "invalid depth value." << std::endl;
      }
    }
    else
    {
      std::cout << "perft command takes exactly one argument" << std::endl;
    }
  }
  else if (words[0] == "dperft")
  {
    if (words.size() == 2)
    {
      int depth{};
      try
      {
        depth = std::stoi(words[1]);
      }
      catch (const std::exception &e)
      {
        std::cout << "invalid depth value." << std::endl;
        return;
      }

      if (depth >= 0)
      {
        dperftCommand(board, depth);
      }
      else
      {
        std::cout << "invalid depth value." << std::endl;
      }
    }
    else
    {
      std::cout << "dperft command takes exactly one argument" << std::endl;
    }
  }
  else
  {
    std::cout << "unknown command, type 'help' for more the available commands"
              << std::endl;
  }
}

void helpCommand()
{
  std::cout
      << "ascii                 Toggles between ascii and utf-8 board representation\n"
      << "cc                    Plays computer-to-computer [TODO]\n"
      << "display               Displays board \n"
      << "dperft (n)            Divided perft\n"
      << "eval                  Shows static evaluation of this position [TODO]\n"
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

void infoCommand(const Board &board)
{
  std::string fen = board.getFen();
  std::vector<std::string> splitted_fen = Utils::tokenizeString(fen);
  std::cout << "Side to Play                 = " << splitted_fen[1]
            << "\nCastling Rights              = " << splitted_fen[2]
            << "\nEn-passant Square            = " << splitted_fen[3]
            << "\nFifty Move Count             = " << splitted_fen[4]
            << "\nFull Move Number             = " << splitted_fen[5];
  std::cout << "\nOccupied Squares:\n";
  Utils::printBB(board.getOccupancies(BOTH));
  U64 attacked_squares = ZERO;
  for (int sq = A1; sq < N_SQUARES; sq++)
  {
    if (board.isSquareAttacked(sq, board.getSideToMove()))
    {
      attacked_squares |= Tables::SQUARE_BB[sq];
    }
  }
  std::cout << "Attacked Squares:\n";
  Utils::printBB(attacked_squares);
}

void movesCommand(Board &board)
{
  int size = 0;
  for (Move const &move : Movegen::generateLegalMoves(board))
  {
    std::cout << move.getUCI() << "\n";
    Board board_copy = board;
    board_copy.makeMove(move);
    board_copy.display();
    getchar();
    size++;
  }
  std::cout << "Total number of moves: " << size << std::endl;
}

void perftCommand(Board &board, int depth)
{
  std::chrono::time_point<std::chrono::system_clock> start = std::chrono::system_clock::now();
  unsigned long long nodes = Perft::perft(board, depth);
  std::chrono::time_point<std::chrono::system_clock> end = std::chrono::system_clock::now();

  std::chrono::duration<double> elapsed_seconds = end - start;
  std::time_t end_time = std::chrono::system_clock::to_time_t(end);

  std::cout << "Found " << nodes << " nodes." << std::endl;
  std::cout << "Finished computation at " << std::ctime(&end_time);
  std::cout << "Elapsed time: " << elapsed_seconds.count() << "s" << std::endl;
  std::cout << "Nodes Per Second: " << nodes / elapsed_seconds.count() << std::endl;
}

void dperftCommand(Board &board, int depth)
{
  unsigned long long total_nodes = 0;

  std::chrono::time_point<std::chrono::system_clock> start = std::chrono::system_clock::now();

  for (Move const &move : Movegen::generatePseudoLegalMoves(board))
  {
    Board backup = board;
    backup.makeMove(move);
    int king_sq = Utils::bitScanForward(backup.getPieces(Utils::getOpponent(backup.getSideToMove()), KING));
    int attacker_side = backup.getSideToMove();
    if (!backup.isSquareAttacked(king_sq, attacker_side))
    {
      unsigned long long nodes = Perft::perft(backup, depth - 1);
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
  std::cout << "Nodes Per Second: " << total_nodes / elapsed_seconds.count() << std::endl;
}

void moveCommand(Board &board, std::string move_uci)
{
  for (Move const &move : Movegen::generateLegalMoves(board))
  {
    if (move.getUCI() == move_uci)
    {
      board.makeMove(move);
      return;
    }
  }
  std::cout << "invalid move" << std::endl;
}