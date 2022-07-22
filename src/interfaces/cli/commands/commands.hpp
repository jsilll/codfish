#pragma once

#include <interfaces/utils.hpp>

namespace cli
{
    class AsciiCommand : public utils::Command
    {
    public:
        void execute([[maybe_unused]] std::vector<std::string> &args, Board &board);
    };

    class CapturesCommand : public utils::Command
    {
    public:
        void execute([[maybe_unused]] std::vector<std::string> &args, Board &board);
    };

    class DisplayCommand : public utils::Command
    {
    public:
        void execute([[maybe_unused]] std::vector<std::string> &args, Board &board);
    };

    class DividedPerftCommand : public utils::Command
    {
    public:
        void execute([[maybe_unused]] std::vector<std::string> &args, Board &board);
    };

    class EvalCommand : public utils::Command
    {
    public:
        void execute([[maybe_unused]] std::vector<std::string> &args, Board &board);
    };

    class ExitCommand : public utils::Command
    {
    public:
        void execute([[maybe_unused]] std::vector<std::string> &args, Board &board);
    };

    class GetFenCommand : public utils::Command
    {
    public:
        void execute([[maybe_unused]] std::vector<std::string> &args, Board &board);
    };

    class HelpCommand : public utils::Command
    {
    public:
        void execute([[maybe_unused]] std::vector<std::string> &args, Board &board);
    };

    class InfoCommand : public utils::Command
    {
    public:
        void execute([[maybe_unused]] std::vector<std::string> &args, Board &board);
    };

    class MagicsCommand : public utils::Command
    {
    public:
        void execute([[maybe_unused]] std::vector<std::string> &args, Board &board);
    };

    class MoveCommand : public utils::Command
    {
    public:
        void execute([[maybe_unused]] std::vector<std::string> &args, Board &board);
    };

    class MovesCommand : public utils::Command
    {
    public:
        void execute([[maybe_unused]] std::vector<std::string> &args, Board &board);
    };

    class NewCommand : public utils::Command
    {
    public:
        void execute([[maybe_unused]] std::vector<std::string> &args, Board &board);
    };

    class PerftCommand : public utils::Command
    {
    public:
        void execute([[maybe_unused]] std::vector<std::string> &args, Board &board);
    };

    class PLMovesCommand : public utils::Command
    {
    public:
        void execute([[maybe_unused]] std::vector<std::string> &args, Board &board);
    };

    class RotateCommand : public utils::Command
    {
    public:
        void execute([[maybe_unused]] std::vector<std::string> &args, Board &board);
    };

    class SetFenCommand : public utils::Command
    {
    public:
        void execute([[maybe_unused]] std::vector<std::string> &args, Board &board);
    };

    class SwitchCommand : public utils::Command
    {
    public:
        void execute([[maybe_unused]] std::vector<std::string> &args, Board &board);
    };

} // namespace cli
