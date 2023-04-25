#include <codchess/codchess.hpp>

using namespace codchess;

int
main([[maybe_unused]] const int argc, [[maybe_unused]] const char **argv) {
    ::codchess::Init();

    auto board = argc == 1 ? Board("8/8/8/8/8/8/3P4/8 w - - 0 1") : Board(argv[1]);

    std::cout << "Legal Moves:\n";
    for (const auto &move : movegen::Legal(board)) {
        std::cout << move.Uci() << "\n";
    }

    return 0;
}