#pragma once

#include <cstdio>
#include <stdexcept>
#include <string>
#include <string_view>
#include <vector>

namespace engine::str
{
    // Split string at one or more delimiters
    inline std::vector<std::string_view> split(std::string_view string, std::string_view delims = " ")
    {
        using namespace std;

        vector<string_view> tokens;
        for (size_t start = 0; start < string.size(); ) {
            // Find first delimiter 
            const auto end = string.find_first_of(delims, start);
            
            // Add non-empty tokens
            if (start != end)
                tokens.emplace_back(string.substr(start, end-start));
            
            // Break at the end of string
            if (end == string_view::npos)
                break;

            start = end + 1;
        }

        return tokens;
    }

    // Split string at line breaks
    inline std::vector<std::string_view> splitLines(std::string_view string)
    {
        return split(string, "\r\n");
    }

    // Performs sprintf dynamically
    inline std::string format(const char* format, auto... args)
    {
        using namespace std;

        string buffer = string(2048, '\0');
        int len = snprintf(buffer.data(), 2048, format, args...);
        buffer.resize(len);
        if (len >= 2048) {
            len = snprintf(buffer.data(), len, format, args...);
        } else if (len < 0) {
            throw runtime_error(string("[Console] Failed to log message with format: '") + format + "'");
        }

        return buffer;
    }

}