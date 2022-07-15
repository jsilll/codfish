#include <interfaces/uci/commands/ucinewgame.hpp>

#include <engine/movegen/board.hpp>

void interfaces::uci::commands::UCINewGameCommand::execute([[maybe_unused]] std::vector<std::string> &args, Board &board)
{
    board.setStartingPosition();
}
