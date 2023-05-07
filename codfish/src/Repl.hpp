#pragma once

#include <functional>
#include <iostream>
#include <iterator>
#include <string>
#include <unordered_map>

#include <codbrain/codbrain.hpp>

namespace codfish::repl {
/// @brief State of the REPL.
struct State {
    /// @brief Whether the display should be in ASCII or in Unicode.
    bool ascii{true};
    /// @brief The chess brain
    codbrain::Brain brain{6};
    /// @brief Whether the white pieces should be on the bottom.
    bool white_on_bottom{true};
};

typedef std::vector<std::string> Tokens;

typedef std::function<void(State &, const Tokens &)> Command;

typedef std::unordered_map<std::string, Command> Commands;

/// @brief Class that implements the REPL.
class Repl {
  public:
    /// @brief Construct a new Repl object.
    [[nodiscard]] explicit Repl(Commands &&commands) noexcept
        : _commands{std::move(commands)} {}

    /// @brief Run the REPL.
    void Run() noexcept {
        std::string command{};

        while (true) {
            std::cout << "codfish> ";

            std::getline(std::cin, command);
            const auto tokens = Tokenize(command);

            if (tokens.empty()) {
                continue;
            } else if (tokens.front() == "exit") {
                break;
            } else if (_commands.contains(command)) {
                _commands.at(command)(_state, tokens);
            } else {
                std::cerr << "Command not found.\n";
            }

            std::flush(std::cout);
        }
    }

  private:
    /// @brief The state of the REPL.
    State _state{};
    /// @brief The commands.
    Commands _commands;

    /// @brief Tokenize a string.
    /// @param text The string to tokenize.
    /// @return The tokens.
    [[nodiscard]] static Tokens Tokenize(const std::string &text) noexcept {
        std::istringstream iss(text);
        std::vector<std::string> tokens{};
        std::copy(std::istream_iterator<std::string>(iss),
                  std::istream_iterator<std::string>(),
                  std::back_inserter(tokens));
        return tokens;
    }
};
}   // namespace codfish::repl