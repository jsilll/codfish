#include "cli.hpp"

#include <readline/history.h>
#include <readline/readline.h>

#include <algorithm>
#include <chrono>
#include <iostream>
#include <iterator>
#include <regex>
#include <string>
#include <vector>

#include "defs.hpp"
#include "utils.hpp"
#include "magics.hpp"
#include "tables.hpp"
#include "move.hpp"
#include "board.hpp"
#include "movegen.hpp"

char *COMMAND{};
bool ASCII{};

bool parseCommand(std::string buf, Board &board);

void helpCommand();

void infoCommand(const Board &board);

void movesCommand(Board &board);

void perftCommand(Board &board, int depth);

void moveCommand(Board &board, std::string move_uci);

std::vector<std::string> splitString(std::string &str);

void Cli::init()
{
  Magics::init();
  Tables::init();
  Board board = Board(); // TODO: make this global
  while ((COMMAND = readline("> ")) != nullptr)
  {
    if (*COMMAND)
    {
      add_history(COMMAND);
      if (!parseCommand(std::string(COMMAND), board))
      {
        free(COMMAND);
        return;
      }
      free(COMMAND);
    }
  }
}

bool parseCommand(std::string buf, Board &board)
{
  std::vector<std::string> words = splitString(buf);
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
  else if (words[0] == "readfen")
  {
    static const std::regex piece_placements_regex(
        R"((([pnbrqkPNBRQK1-8]{1,8})\/?){8})");
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
      board.setFromFen(words[1], words[2], words[3], words[4], words[5],
                       words[6]);
    }
  }
  else if (words[0] == "printfen")
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
    return false;
  }
  else if (words[0] == "perft")
  {
    if (words.size() == 2)
    {
      int depth = std::stoi(words[1]);
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
  else
  {
    std::cout << "unknown command, type 'help' for more the available commands"
              << std::endl;
  }
  return true;
}

void helpCommand()
{
  std::cout
      << "ascii                 Toggles between ascii and utf-8 board "
         "representation\n"
      << "cc                    Play computer-to-computer \n"
      << "d                     Display board \n"
      << "eval                  Show static evaluation of this position\n"
      << "exit                  Exit program \n"
      << "game                  Show game moves \n"
      << "go                    Computer next move \n"
      << "help                  Show this help \n"
      << "info                  Display variables (for testing purposes)\n"
      << "magics                Generates magic numbers for the bishop and "
         "rook pieces\n"
      << "move e2e4, or h7h8q   Enter a move (use this format)\n"
      << "moves                 Show all legal moves\n"
      << "new                   Start new game\n"
      << "perf                  Benchmark a number of key functions\n"
      << "perft n               Calculate raw number of nodes from here, depth "
         "n \n"
      << "r                     Rotate board \n"
      << "readfen fen           Reads FEN position\n"
      << "printfen              Print current position to FEN \n"
      << "sd n                  Set the search depth to n\n"
      << "undo                  Take back last move\n"
      << "switch                Switches the next side to move\n"
      << std::endl;
}

void infoCommand(const Board &board)
{
  std::string fen = board.getFen();
  std::vector<std::string> splitted_fen = splitString(fen);
  std::cout << "Side to Play                 = " << splitted_fen[1]
            << "\nCastling Rights              = " << splitted_fen[2]
            << "\nEn-passant Square            = " << splitted_fen[3]
            << "\nFifty Move Count             = " << splitted_fen[4]
            << "\nFull Move Number             = " << splitted_fen[5];

  std::cout << "\nOccupied Squares:\n";
  Utils::printBB(board.getOccupancies(BOTH));

  U64 attacked_squares = ZERO;
  for (int sq = A1; sq < N_SQUARES;
       sq++) // generating a BB for all the attacked squares
  {
    if (board.isSquareAttacked(sq, board.getSideToMove()))
    {
      attacked_squares |= Tables::SQUARE_BB[sq];
    }
  }
  std::cout << "Attacked Squares:\n";
  Utils::printBB(attacked_squares);
}

std::vector<std::string> splitString(std::string &text)
{
  std::vector<std::string> words{};
  std::istringstream iss(text);
  copy(std::istream_iterator<std::string>(iss),
       std::istream_iterator<std::string>(), std::back_inserter(words));
  return words;
}

void movesCommand(Board &board)
{
  std::vector<Move> moves = Movegen::generateLegalMoves(board);
  for (auto move : moves)
  {
    std::cout << move.getUCI() << "\n";
    Board board_copy = board;
    board_copy.makeMove(move);
    board_copy.display();
    getchar();
  }
  std::cout << "Total number of moves: " << moves.size() << std::endl;
}

long perft(Board &board, int depth)
{
  if (depth == 0)
  {
    return 1;
  }

  long nodes{};
  Board backup = board;
  for (auto move : Movegen::generatePseudoLegalMoves(board))
  {
    if (board.makeMove(move))
    {
      nodes += perft(backup, depth - 1);
      board = backup; // TODO: fix this??
    }
  }

  return nodes;
}

void perftCommand(Board &board, int depth)
{
  std::chrono::time_point<std::chrono::system_clock> start, end;
  start = std::chrono::system_clock::now();
  long nodes = perft(board, depth);
  std::cout << "Found " << nodes << " nodes." << std::endl;
  end = std::chrono::system_clock::now();
  std::chrono::duration<double> elapsed_seconds = end - start;
  std::time_t end_time = std::chrono::system_clock::to_time_t(end);
  std::cout << "Finished computation at " << std::ctime(&end_time);
  std::cout << "Elapsed time: " << elapsed_seconds.count() << "s" << std::endl;
  std::cout << "Nodes Per Second: " << nodes / elapsed_seconds.count() << std::endl;
}

void moveCommand(Board &board, std::string move_uci)
{
  for (Move move : Movegen::generateLegalMoves(board))
  {
    if (move.getUCI() == move_uci)
    {
      board.makeMove(move);
      return;
    }
  }
  std::cout << "invalid move" << std::endl;
}