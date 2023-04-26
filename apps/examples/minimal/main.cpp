#include <codchess/codchess.hpp>
#include <codchess/perft.hpp>

using namespace codchess;

int
main([[maybe_unused]] const int argc, [[maybe_unused]] const char **argv) {
    ::codchess::Init();

    auto board = Board();

    board.Display(std::cout, false);

    return 0;
}