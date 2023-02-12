#pragma once

#include <interfaces/utils.hpp>

namespace cli
{
    class AsciiCommand : public utils::Command
    {
    public:
        AsciiCommand(Board &board) : Command(board){};
        void execute([[maybe_unused]] std::vector<std::string> &args);
    };

    class CapturesCommand : public utils::Command
    {
    public:
        CapturesCommand(Board &board) : Command(board){};
        void execute([[maybe_unused]] std::vector<std::string> &args);
    };

    class DisplayCommand : public utils::Command
    {
    public:
        DisplayCommand(Board &board) : Command(board){};
        void execute([[maybe_unused]] std::vector<std::string> &args);
    };

    class DividedPerftCommand : public utils::Command
    {
    public:
        DividedPerftCommand(Board &board) : Command(board){};
        void execute([[maybe_unused]] std::vector<std::string> &args);
    };

    class EvalCommand : public utils::Command
    {
    public:
        EvalCommand(Board &board) : Command(board){};
        void execute([[maybe_unused]] std::vector<std::string> &args);
    };

    class ExitCommand : public utils::Command
    {
    public:
        ExitCommand(Board &board) : Command(board){};
        void execute([[maybe_unused]] std::vector<std::string> &args);
    };

    class GetFenCommand : public utils::Command
    {
    public:
        GetFenCommand(Board &board) : Command(board){};
        void execute([[maybe_unused]] std::vector<std::string> &args);
    };

    class HelpCommand : public utils::Command
    {
    public:
        HelpCommand(Board &board) : Command(board){};
        void execute([[maybe_unused]] std::vector<std::string> &args);
    };

    class InfoCommand : public utils::Command
    {
    public:
        InfoCommand(Board &board) : Command(board){};
        void execute([[maybe_unused]] std::vector<std::string> &args);
    };

    class MagicsCommand : public utils::Command
    {
    public:
        MagicsCommand(Board &board) : Command(board){};
        void execute([[maybe_unused]] std::vector<std::string> &args);
    };

    class MoveCommand : public utils::Command
    {
    public:
        MoveCommand(Board &board) : Command(board){};
        void execute([[maybe_unused]] std::vector<std::string> &args);
    };

    class MovesCommand : public utils::Command
    {
    public:
        MovesCommand(Board &board) : Command(board){};
        void execute([[maybe_unused]] std::vector<std::string> &args);
    };

    class NewCommand : public utils::Command
    {
    public:
        NewCommand(Board &board) : Command(board){};
        void execute([[maybe_unused]] std::vector<std::string> &args);
    };

    class PerftCommand : public utils::Command
    {
    public:
        PerftCommand(Board &board) : Command(board){};
        void execute([[maybe_unused]] std::vector<std::string> &args);
    };

    class PLMovesCommand : public utils::Command
    {
    public:
        PLMovesCommand(Board &board) : Command(board){};
        void execute([[maybe_unused]] std::vector<std::string> &args);
    };

    class RotateCommand : public utils::Command
    {
    public:
        RotateCommand(Board &board) : Command(board){};
        void execute([[maybe_unused]] std::vector<std::string> &args);
    };

    class SetFenCommand : public utils::Command
    {
    public:
        SetFenCommand(Board &board) : Command(board){};
        void execute([[maybe_unused]] std::vector<std::string> &args);
    };

    class SwitchCommand : public utils::Command
    {
    public:
        SwitchCommand(Board &board) : Command(board){};
        void execute([[maybe_unused]] std::vector<std::string> &args);
    };

} // namespace cli
