#include "cli.hpp"
#include "uci.hpp"

#include <iostream>
#include <string>

typedef enum
{
  CLI,
  UCI,
} InterfaceMode;

typedef struct
{
  InterfaceMode interface;
} ExecOptions;

ExecOptions parseOptions(int argc, const char *argv[]);

int main(int argc, char const *argv[])
{
  switch (parseOptions(argc, argv).interface)
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
    std::cout << "Chess Engine Initialized in CLI Mode" << std::endl;
    Cli::init();
    break;
  }
  return 0;
}

ExecOptions parseOptions(int argc, const char *argv[])
{
  ExecOptions options{};
  for (int i = 1; i < argc; i++)
  {
    std::string option = std::string(argv[i]);
    if (option == "--cli")
    {
      options.interface = CLI;
    }
    else if (option == "--uci")
    {
      options.interface = UCI;
    }
    else
    {
      std::cerr << "Unknown Option: " << option << "\n";
    }
  }
  return options;
}