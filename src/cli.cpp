#include "cli.hpp"

#include "utils.hpp"
#include "tables.hpp"
#include "magics.hpp"
#include "board.hpp"
#include <algorithm>
#include <cctype>
#include <csignal>
#include <iostream>
#include <iostream>
#include <locale>
#include <readline/history.h>
#include <readline/readline.h>
#include <string>
#include <string>
#include <vector>
#include <vector>

// clang-format off
const char* SQUARE_NAMES[] = {
    "A1","B1","C1","D1","E1","F1","G1","H1",
    "A2","B2","C2","D2","E2","F2","G2","H2",
    "A3","B3","C3","D3","E3","F3","G3","H3",
    "A4","B4","C4","D4","E4","F4","G4","H4",
    "A5","B5","C5","D5","E5","F5","G5","H5",
    "A6","B6","C6","D6","E6","F6","G6","H6",
    "A7","B7","C7","D7","E7","F7","G7","H7",
    "A8","B8","C8","D8","E8","F8","G8","H8",
};
// clang-format on

char *COMMAND{};

bool doCommand(const std::string buf, Board &board);
void infoCommand(const Board &board);

void cli::readCommands()
{
    Board board = Board();

    while (COMMAND = readline("> "))
    {
        if (*COMMAND)
        {
            add_history(COMMAND);
            if (!doCommand(std::string(COMMAND), board))
            {
                free(COMMAND);
                return;
            }
            free(COMMAND);
        }
    }
}

bool doCommand(const std::string buf, Board &board)
{
    if (buf == "help" || buf == "h" || buf == "?")
    {
        std::cout
            << "black                 Black to move\n"
            << "cc                    Play computer-to-computer \n"
            << "d                     Display board \n"
            << "eval                  Show static evaluation of this position\n"
            << "exit                  Exit program \n"
            << "game                  Show game moves \n"
            << "go                    Computer next move \n"
            << "help                  Show this help \n"
            << "info                  Display variables (for testing purposes)\n"
            << "move e2e4, or h7h8q   Enter a move (use this format)\n"
            << "moves                 Show all legal moves\n"
            << "new                   Start new game\n"
            << "perf                  Benchmark a number of key functions\n"
            << "perft n               Calculate raw number of nodes from here, depth n \n"
            << "r                     Rotate board \n"
            << "readfen fen           Reads FEN position\n"
            << "sd n                  Set the search depth to n\n"
            << "setup                 Setup board... \n"
            << "undo                  Take back last move\n"
            << "white                 White to move"
            << "magics                Generates magic numbers for the bishop and rook pieces\n"
            << std::endl;
    }
    else if (buf == "black")
    {
        board._white_to_move = false;
    }
    else if (buf == "d")
    {
        std::cout << board;
    }
    else if (buf == "info")
    {
        infoCommand(board);
    }
    else if (buf == "new")
    {
        board.init();
    }
    else if (buf == "r")
    {
        board._view_rotated = !board._view_rotated;
        std::cout << board;
    }
    else if (buf == "white")
    {
        board._white_to_move = true;
    }
    else if (buf == "exit")
    {
        return false;
    }
    else if (buf == "magics")
    {
        magics::generate();
    }
    else
    {
        std::cout << "command unknown or not implemented: " << buf << ", type 'help' for more the available commands" << std::endl;
    }
    return true;
}

void infoCommand(const Board &board)
{
    std::cout
        << "Size of board, in bytes   = " << sizeof(board)
        << "\nMaterial value            = " << board._material
        << "\nWhite castling rights     = " << board._castle_white
        << "\nBlack castling rights     = " << board._castle_black
        << "\nEn-passant square         = " << board._ep_square
        << "\nFifty move count          = " << board._fifty_move
        << "\nbitCnt of white pawns     = " << utils::bitCount(board._white_pawns)
        << "\nbitCnt of black pawns     = " << utils::bitCount(board._black_pawns)
        << "\nbitmap of board._occupied_squares:" << std::endl;
    utils::printBB(board._occupied_squares);
}

void loadFenCommand(const Board &board)
{
}
