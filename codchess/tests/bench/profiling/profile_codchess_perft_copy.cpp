#include <codchess/perft.hpp>

using namespace cod;

int main() {
    constexpr auto depth = 5;
    auto board = chess::Board();
    chess::perft::PerftCopy(board, depth);
    return EXIT_SUCCESS;
}