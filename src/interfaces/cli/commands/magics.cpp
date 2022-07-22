#include <interfaces/cli/commands/commands.hpp>

#include <engine/movegen/magics.hpp>

void cli::MagicsCommand::execute([[maybe_unused]] std::vector<std::string> &args, [[maybe_unused]] Board &board)
{
    magics::generate();
}