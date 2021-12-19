#pragma once

#include "platform/Window.h"
#include "render/Render.h"
#include "render/pipelines/RenderPipeline.h"
#include "render/pipelines/forward/Forward.h"

namespace engine
{
    class Engine
    {
    private:
        Window* window;
        render::Render render;
        render::ForwardRenderPipeline renderPipeline;

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
            renderPipeline.Init(render);
        }

        void Start()
        {
            while (!window->ShouldClose()) {
                window->PreUpdate();
                render.BeginFrame();
                
                renderPipeline.RenderFrame(render);

                render.EndFrame();
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