#include <codchess/codchess.hpp>

using namespace cod;

int
main([[maybe_unused]] const int argc, [[maybe_unused]] const char **argv) {
    chess::Init();

    std::string input;
    auto board = chess::Board();

    while (true) {
        board.Display(std::cout, false);

        std::getline(std::cin, input);

        if (input == "quit") {
            break;
        } else {
            const auto moves = chess::movegen::Legal(board);

            bool found{false};
            for (const auto move : moves) {
                if (move.ToString() == input) {
                    board.Make(move);
                    found = true;
                    break;
                }
            }

            if (!found and !moves.empty()) {
                board.Make(*moves.begin());
            }
        }
    }

    return EXIT_SUCCESS;
}