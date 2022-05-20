#pragma once

#ifdef _WIN32
    #define PLATFORM_WINDOWS 1
    #define PLATFORM_WIN     1
#else
    #define PLATFORM_POSIX   1
    #define PLATFORM_LINUX   1
    #define PLATFORM_X11     1
#endif

#define PLATFORM_SDL     1
#define PLATFORM_BGFX    1

