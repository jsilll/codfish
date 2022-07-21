#pragma once

#include <interfaces/utils.hpp>

#include <optional>

class Move;

namespace interfaces::uci::commands
{
    /**
     * @brief Handles the UCI 'position' command
     *
     */
    class PositionCommand : public interfaces::utils::Command
    {
    public:
        void execute(std::vector<std::string> &args, Board &board);

    private:
        void handleMoves(std::vector<std::string> &moves, Board &board);
        void handleFenPosition(std::string &piece_placements, std::string &active_color, std::string &castling_rights, std::string &en_passant, std::string &halfmove_clock, std::string &fullmove_number, Board &board);
        std::optional<Move> parseMove(std::string move_uci, Board &board);
    };

} // namespace interfaces::uci::commands
