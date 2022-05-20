#include "uci.hpp"

#include "utils.hpp"
#include "magics.hpp"
#include "tables.hpp"
#include "board.hpp"
#include "move.hpp"
#include "movelist.hpp"
#include "movegen.hpp"
#include "ai.hpp"
#include "eval.hpp"

#include <iostream>
#include <optional>
#include <regex>
#include <string>
#include <vector>

/**
 * @brief Command Abstract class
 * All commands should implement this class
 *
 */
class Command
{
public:
    virtual ~Command() {}
    virtual void execute(std::vector<std::string> &args, Board &board) = 0;
};

/**
 * @brief Handles display command
 *
 */
class DisplayCommand : public Command
{
public:
    void execute(std::vector<std::string> &args, Board &board)
    {
        board.display();
    }
};

/**
 * @brief Handles the UCI 'uci' command
 *
 */
class UCICommand : public Command
{
public:
    void execute(std::vector<std::string> &args, Board &board)
    {
        std::cout << "id name Chess Engine\n";
        std::cout << "id name Chess Engine\n";
        std::cout << "uciok" << std::endl;
    }
};

/**
 * @brief Handles the UCI 'isready' command
 *
 */
class IsReadyCommand : public Command
{
public:
    void execute(std::vector<std::string> &args, Board &board)
    {
        std::cout << "readyok" << std::endl;
    }
};

/**
 * @brief Handles the UCI 'ucinewgame' command
 *
 */
class UCINewGameCommand : public Command
{
public:
    void execute(std::vector<std::string> &args, Board &board)
    {
        board.setStartingPosition();
    }
};

/**
 * @brief Handles the UCI 'position' command
 *
 */
class PositionCommand : public Command
{
public:
    void execute(std::vector<std::string> &args, Board &board)
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
                handleFenPosition(args[0], args[1], args[2], args[3], args[4], args[5], board);
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

            handleMoves(args, board);
        }
    }

private:
    void handleMoves(std::vector<std::string> &moves, Board &board)
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

    void handleFenPosition(std::string &piece_placements, std::string &active_color, std::string &castling_rights, std::string &en_passant, std::string &halfmove_clock, std::string &fullmove_number, Board &board)
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

    std::optional<Move> parseMove(std::string move_uci, Board &board)
    {
        for (Move const &move : Movegen::generateLegalMoves(board))
        {
            if (move.getUCI() == move_uci)
            {
                return move;
            }
        }

        return std::nullopt;
    }
};

/**
 * @brief Handles the UCI 'go' command
 *
 */
class GoCommand : public Command
{
public:
    void execute(std::vector<std::string> &args, Board &board)
    {
        int depth = 4;

        if (args.size() != 0 && args[0] == "depth")
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

                if (depth < 0)
                {
                    return;
                }

                std::cout << depth << "\n";
            }
            args.erase(args.begin());
        }

        // TODO: only instatiate AI once
        AI ai = AI(board);
        ai.setDepth(depth);
        AI::SearchResult result = ai.find_best_move();
        std::cout << "info score cp " << result.score << " depth " << depth << " nodes " << result.nodes << "\n";
        std::cout << "best_move " << Move(result.best_move_encoded).getUCI() << std::endl;
    }
};

/**
 * @brief Handles the UCI 'exit' command
 *
 */
class QuitCommand : public Command
{
public:
    void execute(std::vector<std::string> &args, Board &board)
    {
        exit(EXIT_SUCCESS);
    }
};

/**
 * @brief Processes incoming commands
 *
 */
class CommandProcessor
{
private:
    std::unordered_map<std::string, std::unique_ptr<Command>> _mapper;
    Board _board = Board();

public:
    static CommandProcessor &Instance()
    {
        static CommandProcessor cp;
        return cp;
    }

    void init()
    {
        std::string cmd;
        while (std::getline(std::cin, cmd))
        {
            std::vector<std::string> args = Utils::tokenizeString(cmd);

            if (args.size() == 0)
            {
                continue;
            }

            auto command = _mapper.find(args[0]);
            if (command != _mapper.end())
            {
                args.erase(args.begin());
                command->second->execute(args, _board);
            }
        }
    }

private:
    CommandProcessor()
    {
        _mapper["uci"] = std::unique_ptr<Command>(new UCICommand);
        _mapper["isready"] = std::unique_ptr<Command>(new IsReadyCommand());
        _mapper["ucinewgame"] = std::unique_ptr<Command>(new UCINewGameCommand());
        _mapper["position"] = std::unique_ptr<Command>(new PositionCommand());
        _mapper["d"] = std::unique_ptr<Command>(new DisplayCommand());
        _mapper["go"] = std::unique_ptr<Command>(new GoCommand());
        _mapper["quit"] = std::unique_ptr<Command>(new QuitCommand());
    }
};

void Uci::init()
{
    Magics::init();
    Tables::init();
    Eval::init();
    CommandProcessor::Instance().init();
}