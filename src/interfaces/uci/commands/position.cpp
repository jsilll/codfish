#include <interfaces/uci/commands/commands.hpp>

#include <engine/move.hpp>

#include <engine/movegen/movegen.hpp>

#include <optional>

static std::optional<Move> parse_mov(std::string move_uci, Board &board)
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

static void handle_moves(std::vector<std::string> &moves, Board &board)
{
    for (std::string move_uci : moves)
    {
        std::optional<Move> parsed_move = parse_mov(move_uci, board);
        if (parsed_move.has_value())
        {
            board.make_move(parsed_move.value());
        }
    }
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

        std::vector<std::string> fen_string(args.begin(), args.begin() + 6);
        if (!utils::is_fen_valid(fen_string))
        {
            return;
        }
        else
        {
            board.set_from_fen(args[0], args[1], args[2], args[3], args[4], args[5]);
            args.erase(args.begin(), args.begin() + 6);
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

        handle_moves(args, board);
    }
}
