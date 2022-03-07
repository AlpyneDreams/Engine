
#include <cstdio>
#include <stdexcept>
#include <string>

#include <raylib.h>

#include "common/Common.h"
#include "platform/Platform.h"
#include "platform/Window.h"

namespace engine
{
    class WindowRaylib final : public Window
    {
        friend Window* Window::CreateWindow();
        uint width, height;
        const char* title;

    public:
        ~WindowRaylib() { CloseWindow(); }

        void Create(const char* name, uint w, uint h, bool resizable)
        {
            width = w, height = h, title = name;
            InitWindow(w, h, name);
            SetWindowState(FLAG_WINDOW_RESIZABLE);
        }

        bool ShouldClose()
        {
            return WindowShouldClose();
        }

        void PreUpdate() {}

        void Update() {}

        std::pair<int, int> GetSize()
        {
            return {width, height};
        }

        const char* GetTitle()
        {
            return title;
        }

        void* GetHandle() { return GetWindowHandle(); }
    #ifdef PLATFORM_X11
        void* GetNativeDisplay() { return nullptr; }
        void* GetNativeWindow() { return nullptr; }
    #endif
    };

    Window* Window::CreateWindow()
    {
        return new WindowRaylib();
    }

    void Window::Shutdown()
    {
        // No shutdown
    }
}
