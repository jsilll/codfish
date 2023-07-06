#include <codchess/perft.hpp>

using namespace cod;

int main() {
    auto board = chess::Board();
    chess::perft::PerftUnmake(board, 5);
    return EXIT_SUCCESS;
}