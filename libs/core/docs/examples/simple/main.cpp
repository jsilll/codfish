#include <algorithm>

#include <core/codchess.hpp>

using namespace codfish;

int
main([[maybe_unused]] const int argc, [[maybe_unused]] const char **argv) {
    core::Init();
    std::string input{};
    auto board = core::Board();
    while (true) {
        board.Display(std::cout, core::Board::DisplayType::Unicode);
        std::getline(std::cin, input);
        if (input == "quit") {
            break;
        } else {
            const auto moves = core::movegen::Legal(board);
            const auto move = std::find_if(moves.begin(), moves.end(),
                                           [&input](const auto &m) {
                                               return m.ToString() == input;
                                           });
            if (move != moves.end()) {
                board.Make(*move);
            } else {
                std::cout << "Invalid move" << std::endl;
            }
        }
    }
    return EXIT_SUCCESS;
}