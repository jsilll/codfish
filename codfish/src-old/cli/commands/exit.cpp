#include "commands.hpp"

void
cli::ExitCommand::execute([[maybe_unused]] std::vector<std::string> &args) {
    exit(EXIT_SUCCESS);
}