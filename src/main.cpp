#include "cli.hpp"
#include "uci.hpp"

#include <iostream>
#include <string>
#include <vector>

typedef enum
{
  UCI,
  CLI,
} InterfaceMode;

typedef struct
{
  InterfaceMode interface;
} ExecOptions;

ExecOptions parseOptions(std::vector<std::string> arguments);

int main(int argc, char const *argv[])
{
  std::vector<std::string> arguments(argv + 1, argv + argc);

  ExecOptions options = parseOptions(arguments);

  switch (options.interface)
  {
  case CLI:
    std::cout << "Chess Engine Initialized in CLI Mode" << std::endl;
    Cli::init();
    break;
  case UCI:
    std::cout << "Chess Engine Initialized in UCI Mode" << std::endl;
    Uci::init();
    break;
  default:
    std::cout << "Chess Engine Initialized in UCI Mode" << std::endl;
    Uci::init();
    break;
  }

  return 0;
}

ExecOptions parseOptions(std::vector<std::string> arguments)
{
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