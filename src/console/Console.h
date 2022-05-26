#pragma once

#include <cstdio>
#include <vector>

// TODO: fmtlib compile-time format strings?
#include <fmt/core.h>
#include <fmt/format.h>
#include <fmt/ostream.h>

namespace engine
{
    namespace GUI { struct ConsoleWindow; }
    
    // The global Console instance.
    extern inline struct Console Console;

    struct Console
    {
    public:
        // TODO: Text styles for various log levels
        void Log(auto format = "", auto... args)      { Print(format, args...); }
        void Warning(auto format = "", auto... args)  { Print(format, args...); }
        void Warn(auto format = "", auto... args)     { Warning(format, args...); }
        void Error(auto format = "", auto... args)    { Print(format, args...); }

        void Print(auto string = "") { Log("{}", string); }

        template <typename... Args> // MSVC doesn't like auto...
        void Print(const char* format = "", Args... args)
        {
            std::string str = Format(format, args...);
            log.push_back(str);
            std::puts(str.c_str());
            newline = true;
        }

        void Printf(const char* format = "", auto... args)
        {
            std::string str = Format(format, args...);
            if (newline || log.size() <= 0) {
                log.push_back(str);
                newline = false;
            } else {
                log.back() += str;
            }
            std::printf("%s", str.c_str());
        }

        template <typename... Args> // MSVC doesn't like auto...
        inline std::string Format(const char* format = "", Args... args)
        try {
            return fmt::format(fmt::runtime(format), args...);
        } catch (fmt::format_error const& err) {
            Error("Cannot format log message: {}", err.what());
            return format;
        }
        
        void Clear()
        {
            log.resize(0);
            newline = true;
        }

        void Execute(const char* string);
    protected:
        std::vector<std::string> log;
        bool newline = true;

        friend struct GUI::ConsoleWindow;
        friend struct ConCommand;
    };

    inline struct Console Console;
}
