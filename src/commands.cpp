#include "commands.hpp"
#include <iostream>
#include <string>
#include <readline/history.h>
#include <readline/readline.h>
#include "board.hpp"
#include "utils.hpp"

bool doCommand(const std::string buf, board::Board &board);
void info(const board::Board &board);

void cli::readCommands() // TODO: tab completion
{
    board::Board board = board::Board();
    board.init();

    char *line;
    while (line = readline("> "))
    {
        if (strlen(line) <= 0)
        {
            break;
        }
        std::string command = std::string(line);
        add_history(line);
        free(line);
        doCommand(command, board);
    }
}

bool doCommand(const std::string buf, board::Board &board)
{
    if (buf == "help" || buf == "h" || buf == "?")
    {
        std::cout << std::endl
                  << "help:" << std::endl;
        std::cout << "black               : BLACK to move" << std::endl;
        std::cout << "cc                  : play computer-to-computer " << std::endl;
        std::cout << "d                   : display board " << std::endl;
        std::cout << "exit                : exit program " << std::endl;
        std::cout << "eval                : show static evaluation of this position" << std::endl;
        std::cout << "game                : show game moves " << std::endl;
        std::cout << "go                  : computer next move " << std::endl;
        std::cout << "help, h, or ?       : show this help " << std::endl;
        std::cout << "info                : display variables (for testing purposes)" << std::endl;
        std::cout << "move e2e4, or h7h8q : enter a move (use this format)" << std::endl;
        std::cout << "moves               : show all legal moves" << std::endl;
        std::cout << "new                 : start new game" << std::endl;
        std::cout << "perf                : benchmark a number of key functions" << std::endl;
        std::cout << "perft n             : calculate raw number of nodes from here, depth n " << std::endl;
        std::cout << "r                   : rotate board " << std::endl;
        std::cout << "readfen filename n  : reads #-th FEN position from filename" << std::endl;
        std::cout << "sd n                : set the search depth to n" << std::endl;
        std::cout << "setup               : setup board... " << std::endl;
        std::cout << "undo                : take back last move" << std::endl;
        std::cout << "white               : WHITE to move" << std::endl;
        std::cout << std::endl;
        return true;
    }

    if (buf == "black")
    {
        board._next_move = false;
        return true;
    }

    if (buf == "d")
    {
        board.display();
        return true;
    }

    if ((buf == "exit") || (buf == "quit"))
    {
        return false;
    }

    if (buf == "info")
    {
        info(board);
        return true;
    }

    if (buf == "new")
    {
        board.init();
        return true;
    }

    if (buf == "r")
    {
        board._view_rotated = !board._view_rotated;
        board.display();
        return true;
    }

    if (buf == "white")
    {
        board._next_move = true;
        return true;
    }

    std::cout << "    command not implemented: " << buf << ", type 'help' for more the available commands" << std::endl;
    return true;
}

void info(const board::Board &board)
{
    //  your playground... display variables - meant for testing/verification purposes only
    std::cout << std::endl
              << "============ info start ==============" << std::endl;
    std::cout << "Size of board, in bytes   = " << sizeof(board) << std::endl;
    std::cout << "Material value            = " << board._material << std::endl;
    std::cout << "White castling rights     = " << board._castle_white << std::endl;
    std::cout << "Black castling rights     = " << board._castle_black << std::endl;
    std::cout << "En-passant square         = " << board._ep_square << std::endl;
    std::cout << "Fifty move count          = " << board._fifty_move << std::endl;

    std::cout << "bitCnt of white pawns     = " << utils::bitCnt(board._white_pawns) << std::endl;
    std::cout << std::endl
              << "bitmap of _occupied_squares:" << std::endl;
    utils::print_bb(board._occupied_squares);
    std::cout << "============ info end ================" << std::endl
              << std::endl;

    return;
}