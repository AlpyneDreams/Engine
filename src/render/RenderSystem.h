#pragma once

#include "common/Event.h"
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

        Event<render::Render&> OnBeginFrame;
        Event<render::Render&> OnEndFrame;
        Event<render::RenderContext&> OnBeginCamera;
        Event<render::RenderContext&> OnEndCamera;

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
            using namespace render;
            GUI::Update();
            
            Render& r = *render;

            r.BeginFrame();
            OnBeginFrame(r);

            // Render each camera with the render pipeline
            for (auto&& [ent, camera] : World.Each<Camera>())
            {
                RenderContext ctx = RenderContext(r, camera);

                OnBeginCamera(ctx);
                renderPipeline.Render(ctx);
                OnEndCamera(ctx);
            }
            
            OnEndFrame(r);
            r.EndFrame();

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