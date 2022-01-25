#include "cli.hpp"
#include <iostream>
#include <string>
#include <readline/readline.h>
#include <readline/history.h>
#include "board.hpp"
#include "utils.hpp"

bool doCommand(const std::string buf, Board &board);
void infoCommand(const Board &board);

void cli::readCommands() // TODO: tab completion
{
    Board board = Board();
    while (char *command = readline("> "))
    {
        add_history(command);
        if (!doCommand(std::string(command), board))
            return;
        free(command);
    }
}

bool doCommand(const std::string buf, Board &board)
{
    if (buf == "help" || buf == "h" || buf == "?")
    {
        std::cout << "\n"
                  << "help                : Displays information about all the commands\n"
                  << "black               : BLACK to move\n"
                  << "cc                  : play computer-to-computer \n"
                  << "d                   : display board \n"
                  << "exit                : exit program \n"
                  << "eval                : show static evaluation of this position\n"
                  << "game                : show game moves \n"
                  << "go                  : computer next move \n"
                  << "help                : show this help \n"
                  << "info                : display variables (for testing purposes)\n"
                  << "move e2e4, or h7h8q : enter a move (use this format)\n"
                  << "moves               : show all legal moves\n"
                  << "new                 : start new game\n"
                  << "perf                : benchmark a number of key functions\n"
                  << "perft n             : calculate raw number of nodes from here, depth n \n"
                  << "r                   : rotate board \n"
                  << "readfen filename n  : reads #-th FEN position from filename\n"
                  << "sd n                : set the search depth to n\n"
                  << "setup               : setup board... \n"
                  << "undo                : take back last move\n"
                  << "white               : WHITE to move"
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
    else if ((buf == "exit"))
    {
        return false;
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
    utils::print_bb(board._occupied_squares);
    std::cout << "============ info end ================\n";
}