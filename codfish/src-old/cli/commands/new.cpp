#include "commands.hpp"

void cli::NewCommand::execute([[maybe_unused]] std::vector<std::string> &args)
{
    _board.SetStartingPosition();
}