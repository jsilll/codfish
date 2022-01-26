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

bool doCommand(const std::string buf, Board &board);
void infoCommand(const Board &board);
char **commandCompletion(const char *text, int start, int end);
char *commandMatchGenerator(const char *text, int state);

// trim from start (in place)
static inline void ltrim(std::string &s)
{
    s.erase(s.begin(), std::find_if(s.begin(), s.end(), [](unsigned char ch)
                                    { return !std::isspace(ch); }));
}

// trim from end (in place)
static inline void rtrim(std::string &s)
{
    s.erase(std::find_if(s.rbegin(), s.rend(), [](unsigned char ch)
                         { return !std::isspace(ch); })
                .base(),
            s.end());
}

// trim from both ends (in place)
static inline void trim(std::string &s)
{
    ltrim(s);
    rtrim(s);
}

// trim from both ends (copying)
static inline std::string trim_copy(std::string s)
{
    trim(s);
    return s;
}

void cli::readCommands()
{
    Board board = Board();
    rl_attempted_completion_function = commandCompletion;
    while (char *command = readline("> "))
    {
        if (*command)
        {
            add_history(command);
            if (!doCommand(trim_copy(std::string(command)), board))
            {
                free(command);
                return;
            }
            free(command);
        }
    }
}

char **commandCompletion(const char *text, int start, int end)
{
    rl_attempted_completion_over = 1;
    return rl_completion_matches(text, commandMatchGenerator);
}

char *commandMatchGenerator(const char *text, int state)
{
    static const std::vector<std::string> commands{
        "help",
        "black",
        "cc",
        "d",
        "exit",
        "eval",
        "game",
        "go",
        "info",
        "move",
        "moves",
        "new",
        "perf",
        "r",
        "readfen",
        "sd",
        "setup",
        "undo",
        "white",
    };

    static std::vector<std::string> matches;
    static size_t match_index = 0;

    if (!state)
    {
        matches.clear();
        match_index = 0;

        std::string textstr(text);
        for (auto word : commands)
        {
            if (word.size() >= textstr.size() && word.compare(0, textstr.size(), textstr) == 0)
            {
                matches.push_back(word);
            }
        }
    }

    if (match_index >= matches.size())
    {
        return nullptr;
    }

    return strdup(matches[match_index++].c_str());
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
    utils::printBB(board._occupied_squares);
    std::cout << "============ info end ================\n";
}