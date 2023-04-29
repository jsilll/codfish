#include <codchess/codchess.hpp>

using namespace codchess;

int
main([[maybe_unused]] const int argc, [[maybe_unused]] const char **argv) {
    ::codchess::Init();

    std::string input;
    auto board = Board();

    while (true) {
        board.Display(std::cout, false);

        std::getline(std::cin, input);

        if (input == "quit") {
            break;
        } else if (input == "go") {
            const auto moves = movegen::Legal(board);
            if (!moves.Empty()) {
                board.Make(*moves.begin());
            }
        } else {
            const auto moves = movegen::Legal(board);
            for (const auto move : moves) {
                if (move.Uci() == input) {
                    board.Make(move);
                }
            }
        }
    }

    return 0;
}