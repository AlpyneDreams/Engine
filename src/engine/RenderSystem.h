#pragma once

#include "platform/Window.h"
#include "render/Render.h"
#include "render/pipelines/RenderPipeline.h"
#include "render/pipelines/forward/Forward.h"
#include "imgui/Common.h"

#include "System.h"

namespace engine
{
    // Manages rendering systems and components
    struct RenderSystem : public System
    {
        Window* window;
        render::Render* render                       = render::Render::Create();
        render::ForwardRenderPipeline renderPipeline = render::ForwardRenderPipeline(*render);

        RenderSystem(Window* win) : window(win) {}

        void Start()
        {
            window->Create("Engine", 1280, 720, true);
            render->Init(window);
            window->OnAttach();
            renderPipeline.Start();
        }

        void Update()
        {
            GUI::ShowDemoWindow();

            render->BeginFrame();
            renderPipeline.Update();
            render->EndFrame();
        }

        void Shutdown()
        {
            window->OnDetach();
            render->Shutdown();
        }

        ~RenderSystem() {
            delete render;
        }
        
    };
}