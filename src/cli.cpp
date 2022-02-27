#include "cli.hpp"

#include "defs.hpp"
#include "utils.hpp"
#include "tables.hpp"
#include "magics.hpp"
#include "board.hpp"
#include "move.hpp"
#include <algorithm>
#include <cctype>
#include <csignal>
#include <iostream>
#include <locale>
#include <readline/history.h>
#include <readline/readline.h>
#include <string>
#include <sstream>
#include <vector>
#include <iterator>
#include <regex>

// clang-format off
// const char* SQUARE_NAMES[] = {
//     "a1","b1","c1","d1","e1","f1","g1","h1",
//     "a2","b2","c2","d2","e2","f2","g2","h2",
//     "a3","b3","c3","d3","e3","f3","g3","h3",
//     "a4","b4","c4","d4","e4","f4","g4","h4",
//     "a5","b5","c5","d5","e5","f5","g5","h5",
//     "a6","b6","c6","d6","e6","f6","g6","h6",
//     "a7","b7","c7","d7","e7","f7","g7","h7",
//     "a8","b8","c8","d8","e8","f8","g8","h8", 
//     "none"
// };
// clang-format on

char *COMMAND{};
bool ASCII{};

bool parseCommand(std::string buf, Board &board);
void helpCommand();
void infoCommand(const Board &board);
std::vector<std::string> splitString(std::string &str);

void Cli::init()
{
    Board board = Board(); // TODO: make this global

    while (COMMAND = readline("> "))
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
        std::cout << "side to play is now " << (board.switchSideToMove() == WHITE ? "white" : "black") << std::endl;
    }
    else if (words[0] == "display" || words[0] == "d")
    {
        board.print(ASCII);
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
        std::cout << (board.rotate() ? "white" : "black") << " is now on bottom" << std::endl;
    }
    else if (words[0] == "readfen")
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
    else if (words[0] == "printfen")
    {
        std::cout << board.getFen();
    }
    else if (words[0] == "magics")
    {
        Magics::generate();
    }
    else if (words[0] == "perf")
    {
        std::vector<Move> moves = board.moves();
        for (auto move : moves)
        {
            std::cout << move;
        }
    }
    else if (words[0] == "ascii")
    {
        std::cout << "ascii mode toggled " << ((ASCII = !ASCII) ? "on" : "off") << std::endl;
    }
    else if (words[0] == "exit")
    {
        return false;
    }
    else
    {
        std::cout << "unknown command, type 'help' for more the available commands" << std::endl;
    }
    return true;
}

void helpCommand()
{
    std::cout
        << "ascii                 Toggles between ascii and utf-8 board representation\n"
        << "cc                    Play computer-to-computer \n"
        << "d                     Display board \n"
        << "eval                  Show static evaluation of this position\n"
        << "exit                  Exit program \n"
        << "game                  Show game moves \n"
        << "go                    Computer next move \n"
        << "help                  Show this help \n"
        << "info                  Display variables (for testing purposes)\n"
        << "magics                Generates magic numbers for the bishop and rook pieces\n"
        << "move e2e4, or h7h8q   Enter a move (use this format)\n"
        << "moves                 Show all legal moves\n"
        << "new                   Start new game\n"
        << "perf                  Benchmark a number of key functions\n"
        << "perft n               Calculate raw number of nodes from here, depth n \n"
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
    std::cout
        << "Side to Play                 = " << splitted_fen[1]
        << "\nCastling Rights              = " << splitted_fen[2]
        << "\nEn-passant Square            = " << splitted_fen[3]
        << "\nFifty Move Count             = " << splitted_fen[4]
        << "\nFull Move Number             = " << splitted_fen[5];

    std::cout << "\nOccupied Squares:\n";
    Utils::printBB(board.getOccupiedSquares());

    U64 attacked_squares = ZERO;
    for (int sq = A1; sq < N_SQUARES; sq++) // generating a BB for all the attacked squares
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
    char delimiter = ' ';
    std::istringstream iss(text);
    copy(std::istream_iterator<std::string>(iss),
         std::istream_iterator<std::string>(),
         std::back_inserter(words));
    return words;
}