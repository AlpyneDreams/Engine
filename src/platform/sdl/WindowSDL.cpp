
#include <cstdio>
#include <stdexcept>
#include <string>

#include <SDL2/SDL.h>
#include <SDL2/SDL_syswm.h>

#include "common/Common.h"
#include "platform/Platform.h"
#include "platform/Window.h"

#if PLATFORM_GL
#include <glad/glad.h>
#endif

namespace engine
{
    class WindowSDL final : public Window
    {
        friend Window* Window::CreateWindow();
        SDL_Window* window;
        bool shouldClose = false;

        SDL_GLContext glContext;

    public:
        ~WindowSDL() { SDL_DestroyWindow(window); }

        void Create(const char* name, uint width, uint height, bool resizable)
        {
        #if PLATFORM_GL
            SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4);
            SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 6);
            SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
        #endif
            int x = SDL_WINDOWPOS_UNDEFINED,
                y = SDL_WINDOWPOS_UNDEFINED;
            int flags = SDL_WINDOW_SHOWN | (resizable ? SDL_WINDOW_RESIZABLE : 0) | (PLATFORM_GL ? SDL_WINDOW_OPENGL : 0);
            
            // Create window!
            window = SDL_CreateWindow(name, x, y, width, height, flags);
            if (!window) {
                throw std::runtime_error("[GLFW] Failed to create window!");
            }

        #if PLATFORM_GL
            // Create GL context
            glContext = SDL_GL_CreateContext(window);
            if (glContext == NULL) {
                throw std::runtime_error(std::string("[SDL] Failed to create GL context! ") + SDL_GetError());
            }

            // Init GLAD
            if (!gladLoadGLLoader(SDL_GL_GetProcAddress)) {
                throw std::runtime_error("[SDL] Failed to initialize GLAD!");
            }

            // Enable VSync
            if (SDL_GL_SetSwapInterval(1) < 0) {
                fprintf(stderr, "[SDL] Warning: Unable to set VSync! %s\n", SDL_GetError());
            }
        #endif
        }

        bool ShouldClose()
        {
            return shouldClose;
        }

        void PreUpdate() {}

        void Update()
        {
        #ifdef PLATFORM_GL
            SDL_GL_SwapWindow(window);
        #endif
            SDL_Event e;
            while (SDL_PollEvent(&e)) {
                switch(e.type) {

                    case SDL_QUIT:
                        shouldClose = true;
                        break;

                    case SDL_WINDOWEVENT: {
                        const SDL_WindowEvent& wev = e.window;
                        switch (wev.event) {
                            case SDL_WINDOWEVENT_SIZE_CHANGED:
                                break;
                            case SDL_WINDOWEVENT_RESIZED:
                                onResize(wev.data1, wev.data2);
                                break;
                            
                            case SDL_WINDOWEVENT_CLOSE:
                                shouldClose = true;
                                break;
                        }
                        break;
                    }

                }
            }
        }

        std::pair<int, int> GetSize()
        {
            int width, height;
            SDL_GetWindowSize(window, &width, &height);
            return {width, height};
        }

        const char* GetTitle()
        {
            return SDL_GetWindowTitle(window);
        }

        void* GetHandle() { return window; }
    #ifdef PLATFORM_X11
        void* GetNativeDisplay() { return GetSysWMInfo().info.x11.display; }
        void* GetNativeWindow() { return (void*)(uintptr_t)GetSysWMInfo().info.x11.window; }
    #endif
    private:
        SDL_SysWMinfo GetSysWMInfo()
        {
            SDL_SysWMinfo wmi;
            SDL_VERSION(&wmi.version);
            if (!SDL_GetWindowWMInfo(window, &wmi)) {
                throw std::runtime_error("[SDL] Failed to get Window system WM info!");
            }
            return wmi;
        }
    };

    Window* Window::CreateWindow()
    {
        // Initialize SDL
        static bool initializedSDL = false;
        if (!initializedSDL)
        {
            int result = SDL_Init(0);
            if (result != 0) {
                throw std::runtime_error(std::string("[SDL] Failed to initialize!", SDL_GetError()));
            }
            initializedSDL = true;
        }

        return new WindowSDL();
    }

    void Window::Shutdown()
    {
        SDL_Quit();
    }
}
