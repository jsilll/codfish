#include <opts.hpp>

#include <interfaces/cli/cli.hpp>
#include <interfaces/uci/uci.hpp>

#include <iostream>

int main(int argc, char const *argv[])
{
  opts::ExecOptions options = opts::parseOptions(argc, argv);

  switch (options.interface)
  {
  case opts::InterfaceMode::CLI:
    std::cout << "Chess Engine Initialized in CLI Mode" << std::endl;
    cli::init();
    break;
  case opts::InterfaceMode::UCI:
    std::cout << "Chess Engine Initialized in UCI Mode" << std::endl;
    uci::init();
    break;
  default:
    std::cout << "Chess Engine Initialized in UCI Mode" << std::endl;
    uci::init();
    break;
  }

  return 0;
}
