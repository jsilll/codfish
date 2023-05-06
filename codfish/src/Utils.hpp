#include <iterator>
#include <regex>
#include <string>
#include <vector>

namespace codfish::utils {
std::vector<std::string>
Tokenize(const std::string &text) {
    std::vector<std::string> tokens{};
    std::istringstream iss(text);
    std::copy(std::istream_iterator<std::string>(iss),
         std::istream_iterator<std::string>(), std::back_inserter(tokens));
    return tokens;
}

bool
ValidFEN(std::vector<std::string> &args) {
    static const std::regex piece_placements_regex(
        R"((([pnbrqkPNBRQK1-8]{1,8})\/?){8})");
    static const std::regex active_color_regex(R"(b|w)");
    static const std::regex castling_rights_regex(R"(-|K?Q?k?q?)");
    static const std::regex en_passant_regex(R"(-|[a-h][3-6])");
    static const std::regex halfmove_clock_regex(R"(\d+)");
    static const std::regex fullmove_number_regex(R"(\d+)");

    return args.size() == 6 and
           std::regex_match(args[0], piece_placements_regex) and
           std::regex_match(args[1], active_color_regex) and
           std::regex_match(args[2], castling_rights_regex) and
           std::regex_match(args[3], en_passant_regex) and
           std::regex_match(args[4], halfmove_clock_regex) and
           std::regex_match(args[5], fullmove_number_regex);
}
}   // namespace codfish::utils
