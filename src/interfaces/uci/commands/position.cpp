#include <interfaces/uci/commands/commands.hpp>

#include <optional>
#include <regex>

#include <engine/move.hpp>

#include <engine/movegen/movegen.hpp>

static std::optional<Move> parseMove(std::string move_uci, Board &board)
{
    for (Move const &move : movegen::generateLegalMoves(board))
    {
        if (move.get_uci() == move_uci)
        {
            return move;
        }
    }

    return std::nullopt;
}

static void handleMoves(std::vector<std::string> &moves, Board &board)
{
    for (std::string move_uci : moves)
    {
        std::optional<Move> parsed_move = parseMove(move_uci, board);
        if (parsed_move.has_value())
        {
            board.make_move(parsed_move.value());
        }
    }
}

static void handleFenPosition(std::string &piece_placements,
                              std::string &active_color,
                              std::string &castling_rights,
                              std::string &en_passant,
                              std::string &halfmove_clock, std::string &fullmove_number,
                              Board &board)
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

    board.set_from_fen(piece_placements, active_color, castling_rights, en_passant, halfmove_clock, fullmove_number);
}

void uci::PositionCommand::execute(std::vector<std::string> &args, Board &board)
{
    if (args.size() == 0)
    {
        return;
    }

    if (args[0] == "startpos")
    {
        args.erase(args.begin());

        board.set_starting_position();
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
