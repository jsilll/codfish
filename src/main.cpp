#include "cli.hpp"
#include "uci.hpp"

#include <iostream>
#include <string>

typedef enum
{
  UCI,
  CLI,
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
    Cli::init();
    break;
  case UCI:
    Uci::init();
    break;
  default:
    Uci::init();
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