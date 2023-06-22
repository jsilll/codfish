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
    /// @brief Whether the debug mode is on.
    bool debug{false};
    /// @brief The chess brain
    codbrain::Brain brain{6};
    /// @brief Whether the white pieces should be on the bottom.
    bool white_on_bottom{true};
};

typedef std::vector<std::string> Tokens;

typedef std::function<void(std::ostream &, State &, const Tokens &)> Command;

typedef std::unordered_map<std::string, Command> Commands;

/// @brief Class that implements the REPL.
class Repl {
  public:
    /// @brief Construct a new Repl object.
    [[nodiscard]] Repl(std::istream &in, std::ostream &out, Commands &&commands,
                       bool prompt) noexcept
        : _in(in), _out(out), _prompt(prompt), _commands{std::move(commands)} {}

    /// @brief Run the REPL.
    [[noreturn]] void Run() noexcept {
        while (true) {
            RunOnce();
        }
    }

    void RunOnce() noexcept {
        std::string command{};

        if (_prompt) {
            _out << "codfish> ";
            std::flush(_out);
        }

        std::getline(_in, command);
        const auto tokens = Tokenize(command);

        if (tokens.empty()) {
            return;
        } else if (tokens.front() == "exit") {
            return;
        } else if (_commands.contains(command)) {
            _commands.at(command)(_out, _state, tokens);
        } else {
            _out << "Command not found.\n";
        }

        std::flush(_out);
    }

  private:
    /// @brief The input stream.
    std::istream &_in;
    /// @brief The output stream.
    std::ostream &_out;
    /// @brief Whether the REPL should prompt the user for input.
    bool _prompt;
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