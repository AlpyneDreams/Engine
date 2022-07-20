#pragma once

// Avoid using #if or #ifdef when possible.
#ifdef _WIN32
    #define PLATFORM_WINDOWS 1
    #define PLATFORM_LINUX   0
    #define PLATFORM_X11     0
#else
    #define PLATFORM_WINDOWS 0
    #define PLATFORM_LINUX   1
    #define PLATFORM_X11     1
#endif

namespace engine
{
    inline constexpr struct Platform
    {
    #ifdef EDITOR
        static constexpr bool Editor  = true;
    #else
        static constexpr bool Editor  = false;
    #endif
        static constexpr bool Windows = PLATFORM_WINDOWS;
        static constexpr bool Linux   = PLATFORM_LINUX;
    } Platform;
}
