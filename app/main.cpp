#include <codbrain/codbrain.hpp>

using namespace codbrain;

int
main([[maybe_unused]] const int argc, [[maybe_unused]] const char *argv[]) {
    ::codbrain::Init();
    SimpleBrain brain;

    try {
        brain.Launch();
        return EXIT_SUCCESS;
    } catch (const std::exception &e) {
        std::cerr << e.what() << '\n';
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}