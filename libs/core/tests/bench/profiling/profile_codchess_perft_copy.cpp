#include <core/perft.hpp>

using namespace codfish;

int main() {
    constexpr auto depth = 5;
    auto board = core::Board();
    core::perft::PerftCopy(board, depth);
    return EXIT_SUCCESS;
}