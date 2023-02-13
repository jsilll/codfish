#pragma once

#include <string>
#include <vector>

namespace opts
{
    typedef enum
    {
        UCI,
        CLI,
    } InterfaceMode;

    typedef struct
    {
        InterfaceMode interface;
    } ExecOptions;

    ExecOptions parseOptions(int argc, char const *argv[]);

} // namespace opts
