#pragma once

#include <interfaces/utils.hpp>

namespace interfaces::uci::commands
{
    /**
     * @brief Handles display command
     *
     */
    class DisplayCommand : public interfaces::utils::Command
    {
    public:
        void execute([[maybe_unused]] std::vector<std::string> &args, Board &board);
    };
}
