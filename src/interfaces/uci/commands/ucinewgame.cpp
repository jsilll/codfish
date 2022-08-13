#include <interfaces/uci/commands/commands.hpp>

void uci::UCINewGameCommand::execute([[maybe_unused]] std::vector<std::string> &args)
{
    _board.set_starting_position();
}
