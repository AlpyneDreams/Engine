#pragma once

#include "Common.h"
#include <cstdio>
#include <stdexcept>
#include <string>
#include <tuple>
#include <filesystem>

namespace engine::fs
{
    inline bool exists(const std::string_view& path)
    {
        return std::filesystem::exists(path.data());
    }

    inline const std::tuple<char*, size_t> readFile(const char* path)
    {
        using namespace std;
        FILE* f = fopen(path, "rb");
        if (!f)
            throw runtime_error(string("[FS] Failed to open file: ") + path);
        fseek(f, 0, SEEK_END);
        long size = ftell(f);
        rewind(f);

        char* buffer = (char*)malloc(size + 1);
        fread(buffer, size, 1, f);
        fclose(f);

        return tuple<char*, size_t>{buffer, size};
    }

    
}