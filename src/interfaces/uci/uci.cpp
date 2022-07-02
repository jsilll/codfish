#include "uci.hpp"

#include "../utils.hpp"

#include "../../engine/defs.hpp"

#include "../../engine/movegen/magics.hpp"
#include "../../engine/movegen/tables.hpp"
#include "../../engine/movegen/board.hpp"
#include "../../engine/movegen/move.hpp"
#include "../../engine/movegen/movelist.hpp"
#include "../../engine/movegen/movegen.hpp"

#include "../../engine/movepicker/movepicker.hpp"
#include "../../engine/movepicker/eval.hpp"

#include <chrono>
#include <iostream>
#include <optional>
#include <regex>
#include <string>
#include <vector>

namespace uci
{
    /**
     * @brief Command Abstract class
     * All commands should implement this class
     *
     */
    class Command
    {
    public:
        virtual void execute(std::vector<std::string> &args, Board &board) = 0;
    };

    /**
     * @brief Handles display command
     *
     */
    class DisplayCommand : public Command
    {
    public:
        void execute([[maybe_unused]] std::vector<std::string> &args, Board &board)
        {
            board.display();
        }
    } displayCommand;

    /**
     * @brief Handles 'the' UCI uci command
     *
     */
    class UCICommand : public Command
    {
    public:
        void execute([[maybe_unused]] std::vector<std::string> &args, [[maybe_unused]] Board &board)
        {
            std::cout << "id name Chess Engine\n";
            std::cout << "id name Chess Engine\n";
            std::cout << "uciok" << std::endl;
        }
    } uciCommand;

    /**
     * @brief Handles the UCI 'isready' command
     *
     */
    class IsReadyCommand : public Command
    {
    public:
        void execute([[maybe_unused]] std::vector<std::string> &args, [[maybe_unused]] Board &board)
        {
            std::cout << "readyok" << std::endl;
        }
    } isReadyCommand;

    /**
     * @brief Handles the UCI 'ucinewgame' command
     *
     */
    class UCINewGameCommand : public Command
    {
    public:
        void execute([[maybe_unused]] std::vector<std::string> &args, Board &board)
        {
            board.setStartingPosition();
        }
    } uciNewGameCommand;

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
            for (Move const &move : movegen::generateLegalMoves(board))
            {
                if (move.getUCI() == move_uci)
                {
                    return move;
                }
            }

            return std::nullopt;
        }
    } positionCommand;

    /**
     * @brief Handles the UCI 'go' command
     *
     */
    class GoCommand : public Command
    {
    public:
        void execute(std::vector<std::string> &args, Board &board)
        {
            int depth = 7;

            if (args.size() != 0 && args[0] == "depth")
            {
                args.erase(args.begin());

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
            // TODO: Refactor AI class to inherit from Board
            MovePicker ai = MovePicker(board);
            ai.setDepth(depth);

            std::chrono::time_point<std::chrono::system_clock> start = std::chrono::system_clock::now();
            MovePicker::SearchResult result = ai.findBestMove();
            std::chrono::time_point<std::chrono::system_clock> end = std::chrono::system_clock::now();

            std::chrono::duration<double> elapsed = end - start;
            std::cout << "info score cp " << result.score
                      << " depth " << depth
                      << " nodes " << result.nodes
                      << " time " << (int)(elapsed / std::chrono::milliseconds(1))
                      << " pv ";

            for (int i = 0; i < result.pv_length; i++)
            {
                std::cout << Move(result.pv[i]).getUCI() << " ";
            }
            std::cout << "\n";

            std::cout << "bestmove " << Move(result.pv[0]).getUCI() << std::endl;
        }
    } goCommand;

    /**
     * @brief Handles the UCI 'exit' command
     *
     */
    class QuitCommand : public Command
    {
    public:
        void execute([[maybe_unused]] std::vector<std::string> &args, [[maybe_unused]] Board &board)
        {
            exit(EXIT_SUCCESS);
        }
    } quitCommand;

    void init()
    {
        magics::init();
        tables::init();
        eval::init();

        Board board = Board();

        std::string line;
        while (std::getline(std::cin, line))
        {
            std::vector<std::string> args = utils::tokenizeString(line);
            std::string cmd = args[0];
            args.erase(args.begin());
            if (cmd == "uci")
            {
                uciCommand.execute(args, board);
            }
            else if (cmd == "isready")
            {
                isReadyCommand.execute(args, board);
            }
            else if (cmd == "ucinewgame")
            {
                uciNewGameCommand.execute(args, board);
            }
            else if (cmd == "position")
            {
                positionCommand.execute(args, board);
            }
            else if (cmd == "d")
            {
                displayCommand.execute(args, board);
            }
            else if (cmd == "go")
            {
                goCommand.execute(args, board);
            }
            else if (cmd == "quit")
            {
                quitCommand.execute(args, board);
            }
        }
    }
} // namespace uci