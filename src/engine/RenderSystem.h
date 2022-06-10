#pragma once

#include "platform/Window.h"
#include "render/Render.h"
#include "render/pipelines/RenderPipeline.h"
#include "render/pipelines/forward/Forward.h"
#include "imgui/Common.h"

#include "entity/Scene.h"
#include "entity/components/Camera.h"

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
            window->Create("Engine", 1920, 1080, true);
            render->Init(window);
            window->OnAttach();
            renderPipeline.Start();
        }

        void Update()
        {
            GUI::Update();

            render->BeginFrame();

            for (auto&& [ent, camera] : World.ents.view<Camera>().each())
            {
                using namespace render;
                Render& r = *render;

                camera.SetMatrices(r);

                renderPipeline.Update();
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