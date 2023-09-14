#include <brain/codbrain.hpp>

using namespace codfish;

int
main([[maybe_unused]] const int argc, [[maybe_unused]] const char *argv[]) {
    brain::Init();
    brain::Mcts brain;

    try {
        brain.Launch();
    } catch (const std::exception &e) {
        std::cerr << e.what() << '\n';
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}