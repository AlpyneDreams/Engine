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

// HACK: Manually set BX_CONFIG_DEBUG so we can use bx lib,
// since the CMake scripts are wrongly defining it as empty.
// Include Platform.h to be able to use <bx/*.h>
#ifdef PLATFORM_BGFX
    #if defined(BX_CONFIG_DEBUG)
        #undef BX_CONFIG_DEBUG
        #define BX_CONFIG_DEBUG 0
    #endif
#endif
