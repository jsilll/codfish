#pragma once

#include <interfaces/utils.hpp>

#include <codchess/movepicker/movepicker.hpp>

namespace uci
{
    class UCICommand : public utils::Command
    {
    public:
        UCICommand(Board &board) : Command(board){};
        void execute([[maybe_unused]] std::vector<std::string> &args);
    };
    class IsReadyCommand : public utils::Command
    {
    public:
        IsReadyCommand(Board &board) : Command(board){};
        void execute([[maybe_unused]] std::vector<std::string> &args);
    };
    class UCINewGameCommand : public utils::Command
    {
    private:
        MovePicker _move_picker;

    public:
        UCINewGameCommand(Board &board, MovePicker &move_picker) : Command(board), _move_picker(move_picker){};
        void execute([[maybe_unused]] std::vector<std::string> &args);
    };
    class PositionCommand : public utils::Command
    {
    private:
        MovePicker &_move_picker;

    public:
        PositionCommand(Board &board, MovePicker &move_picker) : Command(board), _move_picker(move_picker){};
        void execute([[maybe_unused]] std::vector<std::string> &args);
    };
    class DisplayCommand : public utils::Command
    {
    public:
        DisplayCommand(Board &board) : Command(board){};
        void execute([[maybe_unused]] std::vector<std::string> &args);
    };
    class GoCommand : public utils::Command
    {
    private:
        MovePicker &_move_picker;

    public:
        GoCommand(Board &board, MovePicker &move_picker) : Command(board), _move_picker(move_picker){};
        void execute([[maybe_unused]] std::vector<std::string> &args);
    };
    class QuitCommand : public utils::Command
    {
    public:
        QuitCommand(Board &board) : Command(board){};
        void execute([[maybe_unused]] std::vector<std::string> &args);
    };

} // namespace cli
