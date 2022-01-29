#include "cli.hpp"

#include "board.hpp"
#include "utils.hpp"
#include <iostream>
#include <readline/history.h>
#include <readline/readline.h>
#include <string>
#include <vector>
#include <algorithm>
#include <cctype>
#include <locale>
#include <boost/algorithm/string/classification.hpp>
#include <boost/algorithm/string/split.hpp>
#include <iostream>
#include <string>
#include <vector>

bool doCommand(const std::string buf, Board &board);
void infoCommand(const Board &board);

void cli::readCommands()
{
    Board board = Board();
    while (char *command = readline("> "))
    {
        if (*command)
        {
            add_history(command);
            if (!doCommand(std::string(command), board))
            {
                free(command);
                return;
            }
            free(command);
        }
    }
}

bool doCommand(const std::string buf, Board &board)
{
    std::vector<std::string> tokens;
    boost::split(tokens, buf, boost::is_any_of(" "));

    if (tokens.size() == 1)
    {
        if (tokens[0] == "help" || tokens[0] == "h" || tokens[0] == "?")
        {
            std::cout << "\n"
                      << "black               : BLACK to move\n"
                      << "cc                  : play computer-to-computer \n"
                      << "d                   : display board \n"
                      << "eval                : show static evaluation of this position\n"
                      << "exit                : exit program \n"
                      << "game                : show game moves \n"
                      << "go                  : computer next move \n"
                      << "help                : Displays information about all the commands\n"
                      << "help                : show this help \n"
                      << "info                : display variables (for testing purposes)\n"
                      << "move e2e4, or h7h8q : enter a move (use this format)\n"
                      << "moves               : show all legal moves\n"
                      << "new                 : start new game\n"
                      << "perf                : benchmark a number of key functions\n"
                      << "perft n             : calculate raw number of nodes from here, depth n \n"
                      << "r                   : rotate board \n"
                      << "readfen fen         : reads FEN position\n"
                      << "sd n                : set the search depth to n\n"
                      << "setup               : setup board... \n"
                      << "undo                : take back last move\n"
                      << "white               : WHITE to move"
                      << std::endl;
        }
        else if (tokens[0] == "black")
        {
            board._white_to_move = false;
        }
        else if (tokens[0] == "d")
        {
            std::cout << board;
        }
        else if (tokens[0] == "info")
        {
            infoCommand(board);
        }
        else if (tokens[0] == "new")
        {
            board.init();
        }
        else if (tokens[0] == "r")
        {
            board._view_rotated = !board._view_rotated;
            std::cout << board;
        }
        else if (tokens[0] == "white")
        {
            board._white_to_move = true;
        }
        else if ((tokens[0] == "exit"))
        {
            return false;
        }
        else
        {
            std::cout << "command unknown or not implemented: " << buf << ", type 'help' for more the available commands" << std::endl;
        }
    }
    else if (tokens.size() == 7)
    {
        if (tokens[0] == "readfen")
        {
            board.initFromFen(tokens[1].c_str(), tokens[2].c_str(), tokens[3].c_str(), tokens[4].c_str(), tokens[5].c_str(), tokens[6].c_str());
        }
        else
        {
            std::cout << "command unknown or not implemented: " << buf << ", type 'help' for more the available commands" << std::endl;
        }
    }
    else
    {
        std::cout << "command unknown or not implemented: " << buf << ", type 'help' for more the available commands" << std::endl;
    }
    return true;
}

void infoCommand(const Board &board)
{
    std::cout << "\n============ info start ==============\n"
              << "Size of board, in bytes   = " << sizeof(board)
              << "\nMaterial value            = " << board._material
              << "\nWhite castling rights     = " << board._castle_white
              << "\nBlack castling rights     = " << board._castle_black
              << "\nEn-passant square         = " << board._ep_square
              << "\nFifty move count          = " << board._fifty_move
              << "\nbitCnt of white pawns     = " << utils::bitCnt(board._white_pawns)
              << "\nbitCnt of black pawns     = " << utils::bitCnt(board._black_pawns)
              << "\nbitmap of board._occupied_squares:" << std::endl;
    utils::printBB(board._occupied_squares);
    std::cout << "============ info end ================\n";
}

void loadFenCommand(const Board &board)
{
}