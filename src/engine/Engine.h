#pragma once

#include "platform/Window.h"
#include "render/Render.h"

namespace engine
{
    class Engine
    {
    private:
        Window* window;
        Render render;
    public:
        void Init()
        {
        }

        void CreateWindow()
        {
            window = Window::CreateWindow();
            window->Create("Engine", 1280, 720, true);
        }

        void InitRender()
        {
            render.Init(window);
        }

        void Start()
        {
            while (!window->ShouldClose()) {
                window->PreUpdate();
                render.Update();
                window->Update();
            }
        }

        void Shutdown()
        {
            render.Shutdown();
            delete window;
            Window::Shutdown();
        }
    };
}