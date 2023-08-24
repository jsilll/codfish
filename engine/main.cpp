#include <memory>

#include <codbrain/codbrain.hpp>

using namespace cod;

int
main([[maybe_unused]] const int argc, [[maybe_unused]] const char *argv[]) {
    brain::Init();
    auto brain = std::make_unique<brain::Mcts>(); 

    try {
        brain->Launch();
    } catch (const std::exception &e) {
        std::cerr << e.what() << '\n';
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}