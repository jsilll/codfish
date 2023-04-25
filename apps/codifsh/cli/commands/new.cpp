#include <interfaces/cli/commands/commands.hpp>

void cli::NewCommand::execute([[maybe_unused]] std::vector<std::string> &args)
{
    _board.set_starting_position();
}