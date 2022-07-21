#pragma once

#include <interfaces/utils.hpp>

namespace interfaces::uci::commands
{
    /**
     * @brief Handles the UCI 'ucinewgame' command
     *
     */
    class UCINewGameCommand : public interfaces::utils::Command
    {
    public:
        void execute([[maybe_unused]] std::vector<std::string> &args, Board &board);
    };

} // namespace interfaces::uci::commands
