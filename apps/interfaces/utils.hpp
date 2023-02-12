#pragma once

#include <string>
#include <vector>

#include <codlib/board.hpp>

namespace utils
{
    /**
     * @brief Splits a string by spaces
     *
     * @param text
     * @return std::vector<std::string>
     */
    std::vector<std::string> tokenize(const std::string &text);

    /**
     * @brief Checks Whether a given FEN string is valid or not
     *
     * @param args
     * @return true
     * @return false
     */
    bool is_fen_valid(std::vector<std::string> &args);

    /**
     * @brief Command Abstract class
     * All commands should implement this class
     *
     */
    class Command
    {
    protected:
        Board &_board;

    public:
        Command(Board &board) : _board(board){};
        virtual void execute(std::vector<std::string> &args) = 0;
    };

} // namespace utils