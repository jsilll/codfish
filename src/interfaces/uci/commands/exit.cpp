#include <interfaces/uci/commands/commands.hpp>

void uci::QuitCommand::execute([[maybe_unused]] std::vector<std::string> &args)
{
    exit(EXIT_SUCCESS);
}