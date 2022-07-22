#pragma once

#include <string>
#include <vector>

#include <engine/board.hpp>

namespace utils
{
    /**
     * @brief Splits a string by spaces
     *
     * @param text
     * @return std::vector<std::string>
     */
    std::vector<std::string> tokenizeString(const std::string &text);

    /**
     * @brief Command Abstract class
     * All commands should implement this class
     *
     */
    class Command
    {
    public:
        virtual void execute(std::vector<std::string> &args, Board &board) = 0;
    };

} // namespace utils