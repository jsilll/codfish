#include <interfaces/cli/commands/commands.hpp>

#include <engine/movepicker/eval.hpp>

#include <iostream>

void cli::EvalCommand::execute([[maybe_unused]] std::vector<std::string> &args)
{
    std::cout << "Static Evaluation: " << eval::eval(_board) << std::endl;
}
