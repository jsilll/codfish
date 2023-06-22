#include <codfish/cli.hpp>
#include <codfish/uci.hpp>

using namespace codfish;

/// @brief The different modes of the program.
enum Mode {
    /// @brief The command line interface mode.
    CLI,
    /// @brief The UCI mode.
    UCI,
};

/// @brief The options of the program.
struct ProgramOptions {
    /// @brief The mode of the program.
    Mode mode{UCI};
};

/// @brief Parses the options of the program.
/// @param argc The number of arguments
/// @param argv The arguments
/// @return The options
ProgramOptions
ParseOptions(const int argc, char const *argv[]) {
    const auto arguments = std::vector<std::string>(argv + 1, argv + argc);

    ProgramOptions options{};
    for (const auto &arg : arguments) {
        if (arg == "--cli") {
            options.mode = CLI;
        } else if (arg == "--uci") {
            options.mode = UCI;
        } else {
            throw std::runtime_error("Unknown Option" + arg);
        }
    }

    return options;
}

int
main(const int argc, const char *argv[]) {
    try {
        const auto options = ParseOptions(argc, argv);
        switch (options.mode) {
        case CLI:
            repl::Repl(std::cin, std::cout, std::move(cli::commands), true)
                .Run();
        case UCI:
            repl::Repl(std::cin, std::cout, std::move(uci::commands), false)
                .Run();
        }
    } catch (const std::exception &e) {
        std::cerr << e.what() << '\n';
        return 1;
    }

    return 0;
}