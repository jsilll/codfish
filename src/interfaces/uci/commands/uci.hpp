#pragma once

#include <interfaces/utils.hpp>

namespace interfaces::uci::commands
{
    /**
     * @brief Handles 'the' UCI uci command
     *
     */
    class UCICommand : public interfaces::utils::Command
    {
    public:
        void execute([[maybe_unused]] std::vector<std::string> &args, [[maybe_unused]] Board &board);
    };
} // namespace interfaces::uci::commands
