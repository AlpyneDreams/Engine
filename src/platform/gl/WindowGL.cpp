
#include <cstdio>
#include <stdexcept>
#include <string>

#include "common/Common.h"
#include "platform/Platform.h"
#include "platform/Window.h"

#include <GL/OOGL.hpp>

namespace engine
{
    class WindowGL final : public Window
    {
        GL::Window* window;
        const char* title;
    public:
        ~WindowGL()
        {
            window->Close();
            delete window;
        }

        void Create(const char* name, uint width, uint height, bool resizable)
        {
            title = name;
            window = new GL::Window(width, height, name);
        }

        bool ShouldClose()
        {
            return !window->IsOpen();
        }

        void PreUpdate()
        {
            GL::Event ev;
            while (window->GetEvent(ev));
        }

        void Update()
        {
            window->Present();
        }

        std::pair<int, int> GetSize()
        {
            return {window->GetWidth(), window->GetHeight()};
        }

        const char* GetTitle()
        {
            return title;
        }

        void* GetHandle() { return window; }
    #ifdef PLATFORM_X11
        void* GetNativeDisplay() { return nullptr; }
        void* GetNativeWindow() { return nullptr; }
    #endif
    };

    Window* Window::CreateWindow()
    {
        return new WindowGL();
    }

    void Window::Shutdown() {}
}
