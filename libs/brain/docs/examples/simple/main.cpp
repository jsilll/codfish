#include <brain/codbrain.hpp>

using namespace codfish;

int
main([[maybe_unused]] const int argc, [[maybe_unused]] const char **argv) {
    brain::Init();

    std::string input;
    auto board = core::Board();

    while (true) {
        board.Display(std::cout, core::Board::DisplayType::Unicode);

        std::getline(std::cin, input);

        if (input == "quit") {
            break;
        } else {
            const auto moves = core::movegen::Legal(board);

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

    return 0;
}