#include <interfaces/cli/commands/commands.hpp>

void cli::DisplayCommand::execute([[maybe_unused]] std::vector<std::string> &args, Board &board)
{
    board.display();
}