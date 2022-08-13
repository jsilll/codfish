#include <interfaces/uci/commands/commands.hpp>

void uci::DisplayCommand::execute([[maybe_unused]] std::vector<std::string> &args)
{
    _board.display();
}