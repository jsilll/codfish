#include "Cli.hpp"

#include <functional>
#include <iostream>
#include <string>
#include <unordered_map>

#include <codbrain/codbrain.hpp>

#include "Utils.hpp"

namespace codfish::cli {
// -- State --

/// @brief Whether the display should be in ASCII or in Unicode.
bool ascii{true};
/// @brief The chess brain
codbrain::Brain brain{6};
/// @brief Whether the white pieces should be on the bottom.
bool white_on_bottom{true};

// -- Commands --

/// @brief Sets the display to ASCII.
void
AsciiCommand(const std::vector<std::string> &) noexcept {
    std::cout << "Display set to " << ((ascii = !ascii) ? "ASCII" : "Unicode")
              << ".\n";
}

/// @brief Rotates the display.
void
Rotatecommand(const std::vector<std::string> &) noexcept {
    std::cout << "Bottom pieces set to "
              << ((white_on_bottom = !white_on_bottom) ? "white" : "black")
              << ".\n";
}

/// @brief Displays the board.
void
DisplayCommand(const std::vector<std::string> &) noexcept {
    brain.board().Display(std::cout, ascii, white_on_bottom);
}

// -- REPL --

/// @brief The map of commands.
const std::unordered_map<std::string,
                         std::function<void(const std::vector<std::string> &)>>
    commands{
        {"ascii", AsciiCommand},
        {"rotate", Rotatecommand},
        {"display", DisplayCommand},
    };

void
Run() noexcept {
    std::string command;

    while (true) {
        std::cout << "codfish> ";

        std::cin >> command;
        const auto tokens = utils::Tokenize(command);

        if (tokens.empty()) {
            continue;
        } else if (tokens.front() == "exit") {
            break;
        } else if (commands.contains(command)) {
            commands.at(command)(tokens);
        } else {
            std::cerr << "Command not found.\n";
        }

        std::flush(std::cout);
    }
}
}   // namespace codfish::cli