#include "uci.hpp"

int
main([[maybe_unused]] const int argc, [[maybe_unused]] const char *argv[]) {
    try {
        return EXIT_SUCCESS;
    } catch (const std::exception &e) {
        std::cerr << e.what() << '\n';
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}