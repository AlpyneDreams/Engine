#pragma once

#include "platform/Window.h"
#include "render/Render.h"
#include "render/pipelines/RenderPipeline.h"
#include "render/pipelines/Forward.h"
#include "render/RenderContext.h"
#include "imgui/Common.h"

#include "entity/Scene.h"
#include "entity/components/Camera.h"

#include "engine/System.h"

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
            window->Create("Engine", 1920, 1080, true);
            render->Init(window);
            window->OnAttach();
            renderPipeline.Init();
        }

        void Update()
        {
            GUI::Update();

            render->BeginFrame();

            for (auto&& [ent, camera] : World.Each<Camera>())
            {
                using namespace render;
                RenderContext ctx = RenderContext(*render, camera);
                renderPipeline.Render(ctx);
            }
            
            
            render->EndFrame();

            GUI::Render();
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