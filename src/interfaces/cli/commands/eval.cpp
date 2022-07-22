#include <interfaces/cli/commands/commands.hpp>

#include <engine/movepicker/eval.hpp>

#include <iostream>

void cli::EvalCommand::execute([[maybe_unused]] std::vector<std::string> &args, Board &board)
{
    std::cout << "Static Evaluation: " << eval::eval(board) << std::endl;
}
