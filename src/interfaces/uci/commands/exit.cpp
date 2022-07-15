#include <interfaces/uci/commands/exit.hpp>

void interfaces::uci::commands::QuitCommand::execute([[maybe_unused]] std::vector<std::string> &args, [[maybe_unused]] Board &board)
{
    exit(EXIT_SUCCESS);
}