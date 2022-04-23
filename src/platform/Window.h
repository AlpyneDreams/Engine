#pragma once

#include <functional>
#include <utility>

#include "common/Common.h"

namespace engine
{
    class Window
    {
    protected:
        Window() {}

        std::function<void(uint, uint)> onResize;
    public:
        virtual ~Window() {}
        static Window* CreateWindow();
        static void Shutdown();

        virtual void Create(const char* name, uint width, uint height, bool resizable) = 0;

        // Run after Render::Init. Typically confgiures ImGui
        virtual void OnAttach() {}
        // Run before Render::Shutdown. Typically detaches ImGui
        virtual void OnDetach() {}

        virtual bool ShouldClose() = 0;
        virtual void PreUpdate() = 0;
        virtual void Update() = 0;
        virtual std::pair<int, int> GetSize() = 0;
        virtual const char* GetTitle() = 0;

        virtual void* GetHandle() = 0;

        virtual void* GetNativeDisplay() = 0;
        virtual void* GetNativeWindow() = 0;

        void SetResizeCallback(std::function<void(uint, uint)> callback) { onResize = callback; }
    };
}