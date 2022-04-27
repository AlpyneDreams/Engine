#pragma once

#include "common/Common.h"
#include "Console.h"

namespace engine::commands
{
    inline ConCommand echo("echo", "Prints text to console", [](ConCmd& cmd) {
        Console.Log(cmd.args);
    });

    inline ConCommand help("help", "Prints information about a command", [](ConCmd& cmd) {
        if (cmd.argc > 0) {
            ConCommand::PrintHelp(cmd.argv[0]);
        } else {
            ConCommand::PrintCommandList();
        }
    });

    inline ConCommand clear("clear", "Clear console output", [](ConCmd& cmd) {
        Console.Clear();
    });
}

namespace engine::cvars
{
}

