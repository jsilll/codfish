#include <interfaces/cli/commands/commands.hpp>

#include <regex>
#include <iostream>

static bool isFenValid(std::vector<std::string> &args)
{
    static const std::regex piece_placements_regex(R"((([pnbrqkPNBRQK1-8]{1,8})\/?){8})");
    static const std::regex active_color_regex(R"(b|w)");
    static const std::regex castling_rights_regex(R"(-|K?Q?k?q?)");
    static const std::regex en_passant_regex(R"(-|[a-h][3-6])");
    static const std::regex halfmove_clock_regex(R"(\d+)");
    static const std::regex fullmove_number_regex(R"(\d+)");

    return args.size() == 6 &&
           std::regex_match(args[0], piece_placements_regex) &&
           std::regex_match(args[1], active_color_regex) &&
           std::regex_match(args[2], castling_rights_regex) &&
           std::regex_match(args[3], en_passant_regex) &&
           std::regex_match(args[4], halfmove_clock_regex) &&
           std::regex_match(args[5], fullmove_number_regex);
}

void cli::SetFenCommand::execute(std::vector<std::string> &args, Board &board)
{
    if (!isFenValid(args))
    {
        std::cout << "Invalid FEN String" << std::endl;
        return;
    }

    board.set_from_fen(args[0], args[1], args[2], args[3], args[4], args[5]);
}
