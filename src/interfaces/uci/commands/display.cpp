#include <interfaces/uci/commands/display.hpp>

#include <engine/movegen/board.hpp>

void interfaces::uci::commands::DisplayCommand::execute([[maybe_unused]] std::vector<std::string> &args, Board &board)
{
    board.display();
}