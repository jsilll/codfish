#include <codchess/perft.hpp>

using namespace codchess;

int main() {
    auto board = Board();
    perft::PerftUnmake(board, 5);
    return 0;
}