#pragma once

#include "../../utils.hpp"

namespace cli {
class AsciiCommand : public utils::Command {
  public:
    AsciiCommand(codchess::Board &board) : Command(board){};
    void execute([[maybe_unused]] std::vector<std::string> &args);
};

class CapturesCommand : public utils::Command {
  public:
    CapturesCommand(codchess::Board &board) : Command(board){};
    void execute([[maybe_unused]] std::vector<std::string> &args);
};

class DisplayCommand : public utils::Command {
  public:
    DisplayCommand(codchess::Board &board) : Command(board){};
    void execute([[maybe_unused]] std::vector<std::string> &args);
};

class DividedPerftCommand : public utils::Command {
  public:
    DividedPerftCommand(codchess::Board &board) : Command(board){};
    void execute([[maybe_unused]] std::vector<std::string> &args);
};

class EvalCommand : public utils::Command {
  public:
    EvalCommand(codchess::Board &board) : Command(board){};
    void execute([[maybe_unused]] std::vector<std::string> &args);
};

class ExitCommand : public utils::Command {
  public:
    ExitCommand(codchess::Board &board) : Command(board){};
    void execute([[maybe_unused]] std::vector<std::string> &args);
};

class GetFenCommand : public utils::Command {
  public:
    GetFenCommand(codchess::Board &board) : Command(board){};
    void execute([[maybe_unused]] std::vector<std::string> &args);
};

class HelpCommand : public utils::Command {
  public:
    HelpCommand(codchess::Board &board) : Command(board){};
    void execute([[maybe_unused]] std::vector<std::string> &args);
};

class InfoCommand : public utils::Command {
  public:
    InfoCommand(codchess::Board &board) : Command(board){};
    void execute([[maybe_unused]] std::vector<std::string> &args);
};

class MagicsCommand : public utils::Command {
  public:
    MagicsCommand(codchess::Board &board) : Command(board){};
    void execute([[maybe_unused]] std::vector<std::string> &args);
};

class MoveCommand : public utils::Command {
  public:
    MoveCommand(codchess::Board &board) : Command(board){};
    void execute([[maybe_unused]] std::vector<std::string> &args);
};

class MovesCommand : public utils::Command {
  public:
    MovesCommand(codchess::Board &board) : Command(board){};
    void execute([[maybe_unused]] std::vector<std::string> &args);
};

class NewCommand : public utils::Command {
  public:
    NewCommand(codchess::Board &board) : Command(board){};
    void execute([[maybe_unused]] std::vector<std::string> &args);
};

class PerftCommand : public utils::Command {
  public:
    PerftCommand(codchess::Board &board) : Command(board){};
    void execute([[maybe_unused]] std::vector<std::string> &args);
};

class PLMovesCommand : public utils::Command {
  public:
    PLMovesCommand(codchess::Board &board) : Command(board){};
    void execute([[maybe_unused]] std::vector<std::string> &args);
};

class RotateCommand : public utils::Command {
  public:
    RotateCommand(codchess::Board &board) : Command(board){};
    void execute([[maybe_unused]] std::vector<std::string> &args);
};

class SetFenCommand : public utils::Command {
  public:
    SetFenCommand(codchess::Board &board) : Command(board){};
    void execute([[maybe_unused]] std::vector<std::string> &args);
};

class SwitchCommand : public utils::Command {
  public:
    SwitchCommand(codchess::Board &board) : Command(board){};
    void execute([[maybe_unused]] std::vector<std::string> &args);
};

}   // namespace cli
