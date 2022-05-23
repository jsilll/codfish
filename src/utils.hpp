#pragma once

#include <string>
#include <vector>

namespace utils
{
    /**
     * @brief Splits a string by spaces
     *
     * @param text
     * @return std::vector<std::string>
     */
    std::vector<std::string> tokenizeString(const std::string &text);
} // namespace utils