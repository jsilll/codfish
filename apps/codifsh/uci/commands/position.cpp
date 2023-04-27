#include <interfaces/uci/commands/commands.hpp>

#include <codchess/move.hpp>

#include <codchess/movegen/movegen.hpp>

#include <optional>

static std::optional<Move> parse_move(std::string move_uci, Board &board)
{
    for (const auto move : movegen::generateLegalMoves(board))
    {
        if (move.get_uci() == move_uci)
        {
            return move;
        }
    }

    return std::nullopt;
}

static void handle_moves(std::vector<std::string> &moves, Board &board, MovePicker &move_picker)
{
    move_picker.add_to_history(board.get_hash_key()); // For Three-Fold Draws
    for (std::string move_uci : moves)
    {
        std::optional<Move> parsed_move = parse_move(move_uci, board);
        if (parsed_move.has_value())
        {
            Move move = parsed_move.value();
            board.make(move);
            move_picker.add_to_history(board.get_hash_key()); // For Three-Fold Draws
        }
    }
}

void uci::PositionCommand::execute(std::vector<std::string> &args)
{
    if (args.size() == 0)
    {
        return;
    }

    if (args[0] == "startpos")
    {
        args.erase(args.begin());

        _board.set_starting_position();
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
            _board.set_from_fen(args[0], args[1], args[2], args[3], args[4], args[5]);
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

    _move_picker.clear_history();

    if (args[0] == "moves")
    {
        args.erase(args.begin());

        handle_moves(args, _board, _move_picker);
    }

    _move_picker.clear_move_tables();
    _move_picker.clear_transposition_table();
}
