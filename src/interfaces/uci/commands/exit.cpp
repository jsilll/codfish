#include <interfaces/uci/commands/commands.hpp>

#include <engine/movegen/tables.hpp>

void uci::QuitCommand::execute([[maybe_unused]] std::vector<std::string> &args)
{
    tables::teardown();
    exit(EXIT_SUCCESS);
}