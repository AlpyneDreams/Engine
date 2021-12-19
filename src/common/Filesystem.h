#pragma once

#include "Common.h"
#include <cstdio>
#include <stdexcept>
#include <string>
#include <tuple>

namespace engine::fs
{
    const std::tuple<byte*, size_t> readFile(const char* path)
    {
        using namespace std;
        FILE* f = fopen(path, "rb");
        if (!f)
            throw runtime_error(string("[FS] Failed to open file: ", path));
        fseek(f, 0, SEEK_END);
        long size = ftell(f);
        rewind(f);

        byte* buffer = (byte*)malloc(size + 1);
        fread(buffer, size, 1, f);
        fclose(f);

        return tuple<byte*, size_t>{buffer, size};

    }
}