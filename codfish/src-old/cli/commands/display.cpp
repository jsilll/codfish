#include "commands.hpp"

void cli::DisplayCommand::execute([[maybe_unused]] std::vector<std::string> &args)
{
    _board.display();
}