#pragma once

#include <interfaces/utils.hpp>

namespace uci
{
    class UCICommand : public utils::Command
    {
    public:
        void execute([[maybe_unused]] std::vector<std::string> &args, Board &board);
    };
    class IsReadyCommand : public utils::Command
    {
    public:
        void execute([[maybe_unused]] std::vector<std::string> &args, Board &board);
    };
    class UCINewGameCommand : public utils::Command
    {
    public:
        void execute([[maybe_unused]] std::vector<std::string> &args, Board &board);
    };
    class PositionCommand : public utils::Command
    {
    public:
        void execute([[maybe_unused]] std::vector<std::string> &args, Board &board);
    };
    class DisplayCommand : public utils::Command
    {
    public:
        void execute([[maybe_unused]] std::vector<std::string> &args, Board &board);
    };
    class GoCommand : public utils::Command
    {
    public:
        void execute([[maybe_unused]] std::vector<std::string> &args, Board &board);
    };
    class QuitCommand : public utils::Command
    {
    public:
        void execute([[maybe_unused]] std::vector<std::string> &args, Board &board);
    };

} // namespace cli
