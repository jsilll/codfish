#pragma once

#include <interfaces/utils.hpp>

namespace interfaces::uci::commands
{
    /**
     * @brief Handles the UCI 'exit' command
     *
     */
    class QuitCommand : public interfaces::utils::Command
    {
    public:
        void execute([[maybe_unused]] std::vector<std::string> &args, Board &board);
    };
} // namespace interfaces::uci::commands