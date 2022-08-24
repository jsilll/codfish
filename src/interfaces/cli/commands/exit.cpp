#include <interfaces/cli/commands/commands.hpp>

#include <engine/movegen/tables.hpp>

void cli::ExitCommand::execute([[maybe_unused]] std::vector<std::string> &args)
{
    tables::teardown();
    exit(EXIT_SUCCESS);
}