#include <interfaces/uci/commands/commands.hpp>

void uci::QuitCommand::execute([[maybe_unused]] std::vector<std::string> &args, [[maybe_unused]] Board &board)
{
    exit(EXIT_SUCCESS);
}