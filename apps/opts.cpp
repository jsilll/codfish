#include <opts.hpp>

#include <iostream>

namespace opts
{
    ExecOptions parseOptions(int argc, char const *argv[])
    {
        std::vector<std::string> arguments(argv + 1, argv + argc);

        ExecOptions options{};

        for (std::string arg : arguments)
        {
            if (arg == "--cli")
            {
                options.interface = CLI;
            }
            else if (arg == "--uci")
            {
                options.interface = UCI;
            }
            else
            {
                std::cerr << "Unknown Option: " << arg << "\n";
            }
        }

        return options;
    }

} // namespace opts
