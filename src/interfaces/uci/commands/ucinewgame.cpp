#include <interfaces/uci/commands/commands.hpp>

void uci::UCINewGameCommand::execute([[maybe_unused]] std::vector<std::string> &args)
{
    _board.set_starting_position();
    _move_picker.clear_move_tables();
    _move_picker.clear_tranposition_table();
}
