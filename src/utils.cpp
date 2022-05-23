#include "utils.hpp"

#include "tables.hpp"

#include <string>
#include <iostream>
#include <iterator>
#include <algorithm>
#include <regex>
#include <vector>

namespace utils
{
    std::vector<std::string> tokenizeString(const std::string &text)
    {
        std::vector<std::string> tokens{};
        std::istringstream iss(text);
        copy(std::istream_iterator<std::string>(iss), std::istream_iterator<std::string>(), std::back_inserter(tokens));
        return tokens;
    }
} // namespace utils