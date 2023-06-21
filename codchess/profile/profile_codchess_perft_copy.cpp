#include <codchess/perft.hpp>

using namespace codchess;

int main() {
    auto board = Board();
    perft::PerftCopy(board, 5);
    return 0;
}