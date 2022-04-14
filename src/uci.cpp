#include "uci.hpp"

#include "utils.hpp"
#include "magics.hpp"
#include "tables.hpp"
#include "move.hpp"
#include "movelist.hpp"
#include "board.hpp"
#include "movegen.hpp"

#include <iostream>
#include <optional>
#include <regex>
#include <string>
#include <vector>

// Modifies the board state
void parseCommand(std::string &cmd, Board &board);
void positionCommand(std::vector<std::string> &args, Board &board);
void movesSubCommand(std::vector<std::string> moves, Board &board);
void parseFen(std::string &piece_placements, std::string &active_color,
              std::string &castling_rights,
              std::string &en_passant,
              std::string &halfmove_clock,
              std::string &fullmove_number,
              Board &board);
void goCommand(std::vector<std::string> args, Board &board);

// Parsers and Helpers
std::optional<Move> parseMove(std::string move_uci, Board &board);

void Uci::init()
{
    Magics::init();
    Tables::init();

    Board board;

    std::string cmd;
    while (std::getline(std::cin, cmd))
    {
        parseCommand(cmd, board);
    }
}

/**
 * @brief Parses and executes a uci compliant command
 *
 * @param args
 * @param board
 */
void parseCommand(std::string &cmd, Board &board)
{
    std::vector<std::string> args = Utils::tokenizeString(cmd);

    if (args.size() == 0)
    {
        return;
    }

    if (args[0] == "position")
    {
        args.erase(args.begin());

        positionCommand(args, board);
    }
    else if (args[0] == "d")
    {
        board.display();
    }
    else if (args[0] == "go")
    {
        args.erase(args.begin());
        goCommand(args, board);
    }
}

/**
 * @brief Parses and executes position uci command
 *
 * @param args
 * @param board
 */
void positionCommand(std::vector<std::string> &args, Board &board)
{
    if (args.size() == 0)
    {
        return;
    }

    if (args[0] == "startpos")
    {
        args.erase(args.begin());

        board.setStartingPosition();
    }
    else if (args[0] == "fen")
    {
        args.erase(args.begin());

        if (args.size() < 6)
        {
            return;
        }

        try
        {
            parseFen(args[0], args[1], args[2], args[3], args[4], args[5], board);
            args.erase(args.begin(), args.begin() + 6);
        }
        catch (const std::exception &e)
        {
            return;
        }
    }
    else
    {
        return;
    }

    if (args.size() == 0)
    {
        return;
    }

    if (args[0] == "moves")
    {
        args.erase(args.begin());

        movesSubCommand(args, board);
    }
}

/**
 * @brief Parses and executes moves uci subcommand
 *
 * @param moves
 * @param board
 */
void movesSubCommand(std::vector<std::string> moves, Board &board)
{
    for (std::string move_uci : moves)
    {
        std::optional<Move> parsed_move = parseMove(move_uci, board);
        if (parsed_move.has_value())
        {
            board.makeMove(parsed_move.value());
        }
    }
}

/**
 * @brief Parses fen string and sets board accordingly
 *
 * @param piece_placements
 * @param active_color
 * @param castling_rights
 * @param en_passant
 * @param halfmove_clock
 * @param fullmove_number
 * @param board
 */
void parseFen(std::string &piece_placements, std::string &active_color, std::string &castling_rights, std::string &en_passant, std::string &halfmove_clock, std::string &fullmove_number, Board &board)
{
    static const std::regex piece_placements_regex(R"((([pnbrqkPNBRQK1-8]{1,8})\/?){8})");
    static const std::regex active_color_regex(R"(b|w)");
    static const std::regex castling_rights_regex(R"(-|K?Q?k?q?)");
    static const std::regex en_passant_regex(R"(-|[a-h][3-6])");
    static const std::regex halfmove_clock_regex(R"(\d+)");
    static const std::regex fullmove_number_regex(R"(\d+)");
    if (!std::regex_match(piece_placements, piece_placements_regex) ||
        !std::regex_match(active_color, active_color_regex) ||
        !std::regex_match(castling_rights, castling_rights_regex) ||
        !std::regex_match(en_passant, en_passant_regex) ||
        !std::regex_match(halfmove_clock, halfmove_clock_regex) ||
        !std::regex_match(fullmove_number, fullmove_number_regex))
    {
        throw std::invalid_argument("Invalid fen string.");
    }

    board.setFromFen(piece_placements, active_color, castling_rights, en_passant, halfmove_clock, fullmove_number);
}

/**
 * @brief Executes the uci go command
 *
 * @param args
 * @param board
 */
void goCommand(std::vector<std::string> args, Board &board)
{
    if (args.size() == 0)
    {
        return;
    }

    if (args[0] == "depth")
    {
        args.erase(args.begin());
        int depth = 1;

        if (args.size() != 0)
        {
            try
            {
                depth = std::stoi(args[0]);
            }
            catch (const std::exception &e)
            {
                return;
            }

            if (depth > 0)
            {
                std::cout << depth << "\n";
            }
            else
            {
                return;
            }
        }
        args.erase(args.begin());
    }
}

/**
 * @brief Parses move in uci notation
 *
 * @param move_uci
 * @param board
 * @return std::optional<Move>
 */
std::optional<Move> parseMove(std::string move_uci, Board &board)
{
    for (Move move : Movegen::generateLegalMoves(board))
    {
        if (move.getUCI() == move_uci)
        {
            return move;
        }
    }

    return std::nullopt;
}
