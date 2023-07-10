#include <algorithm>

#include <codchess/codchess.hpp>

using namespace cod;

int
main([[maybe_unused]] const int argc, [[maybe_unused]] const char **argv) {
    chess::Init();
    std::string input{};
    auto board = chess::Board();
    while (true) {
        board.Display(std::cout, chess::DisplayType::Unicode);
        std::getline(std::cin, input);
        if (input == "quit") {
            break;
        } else {
            const auto moves = chess::movegen::Legal(board);
            const auto move = std::find_if(moves.begin(), moves.end(),
                                           [&input](const auto &move) {
                                               return move.ToString() == input;
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