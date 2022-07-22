#include <interfaces/cli/commands/commands.hpp>

void cli::ExitCommand::execute([[maybe_unused]] std::vector<std::string> &args, [[maybe_unused]] Board &board)
{
    exit(EXIT_SUCCESS);
}