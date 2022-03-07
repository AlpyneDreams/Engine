#pragma once

#include <string>
#include <string_view>
#include <vector>

namespace engine::str
{
    inline std::vector<std::string_view> split(std::string_view string, std::string_view delims = " ")
    {
        using namespace std;

        vector<string_view> tokens;

        for (size_t start = 0; start < string.size(); ) {

            const auto end = string.find_first_of(delims, start);

            if (start != end)
                tokens.emplace_back(string.substr(start, end-start));
            
            if (end == string_view::npos)
                break;

            start = end + 1;
        }

        return tokens;
    }

    inline std::vector<std::string_view> splitLines(std::string_view string) {
        return split(string, "\r\n");
    }

}