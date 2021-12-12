
#include <cstdio>
#include <stdexcept>

#include "platform/Platform.h"

#ifdef PLATFORM_X11
#define GLFW_EXPOSE_NATIVE_X11
#endif
#include <GLFW/glfw3.h>
#include <GLFW/glfw3native.h>

#include "common/Common.h"
#include "platform/Window.h"

// Callback for all GLFW errors
void errorCallback(int error, const char* msg)
{
    std::fprintf(stderr, "[GLFW] GLFW Error (%d): %s\n", error, msg);
}

void resizeCallback(GLFWwindow* win, int w, int h);

namespace engine
{
    class WindowGLFW final : public Window
    {
        friend Window* Window::CreateWindow();
        GLFWwindow* window;
        int width, height;

    public:
        static bool resizeOccured;

        void Create(const char* name, uint width, uint height, bool resizable)
        {
            // No OpenGL
            glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
            
            // Create window!
            window = glfwCreateWindow(width, height, name, nullptr, nullptr);
            if (!window) {
                throw std::runtime_error("[GLFW] Failed to create window!");
            }

            glfwSetWindowSizeCallback(window, resizeCallback);
        }

        bool ShouldClose()
        {
            return glfwWindowShouldClose(window);
        }
        
        void PreUpdate()
        {
            glfwPollEvents();
        }

        void Update()
        {
            // If window was resized, call onResize
            if (resizeOccured) {
                int oldWidth = width, oldHeight = height;
                glfwGetWindowSize(window, &width, &height);
                if (width != oldWidth || height != oldHeight) {
                    if (onResize)
                        onResize(uint(width), uint(height));
                    resizeOccured = false;
                }
            }
        }

        std::pair<int, int> GetSize()
        {
            int width, height;
            glfwGetWindowSize(window, &width, &height);
            return {width, height};
        }

    #ifdef PLATFORM_X11
        void* GetNativeDisplay() { return glfwGetX11Display(); }
        void* GetNativeWindow() { return (void*)(uintptr_t)glfwGetX11Window(window); }
    #endif
    };

    Window* Window::CreateWindow()
    {
        // Initialize GLFW
        static bool initializedGLFW = false;
        if (!initializedGLFW)
        {
            glfwSetErrorCallback(errorCallback);
            
            if (glfwInit() != GLFW_TRUE) {
                throw std::runtime_error("[GLFW] Failed to initialize!");
            }
            initializedGLFW = true;
        }

        return new WindowGLFW();
    }

    void Window::Shutdown()
    {
        glfwTerminate();
    }

    bool WindowGLFW::resizeOccured = false;
}

void resizeCallback(GLFWwindow* win, int w, int h)
{
    engine::WindowGLFW::resizeOccured = true;
}