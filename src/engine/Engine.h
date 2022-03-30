#pragma once

#include "platform/Window.h"
#include "render/Render.h"
#include "render/pipelines/RenderPipeline.h"
#include "render/pipelines/forward/Forward.h"

#include "Time.h"

namespace engine
{
    // The global engine instance.
    extern inline class Engine Engine;
    
    class Engine
    {
    private:
        Window* window;
        render::Render* render;
        render::ForwardRenderPipeline renderPipeline;

    public:
        void Run()
        {
            Init();
            CreateWindow();
            InitRender();

            // Run the main loop
            Start();

            Shutdown();
        }

    protected:
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
            render = render::Render::Create();
            render->Init(window);
            renderPipeline.Init(*render);
        }

        void Start()
        {
            Time::Seconds lastTime    = Time::GetTime();
            Time::Seconds accumulator = 0;

            while (!window->ShouldClose())
            {
                auto currentTime = Time::GetTime();
                auto deltaTime   = currentTime - lastTime;
                lastTime = currentTime;

                Time.Advance(deltaTime);

                // TODO: Whether we use deltaTime or unscaled.deltaTime affects
                // whether fixed.deltaTime needs to be changed with timeScale.
                // Perhaps the accumulator logic could go into Time.
                accumulator += Time.deltaTime;

                // (Process input)

                while (accumulator >= Time.fixed.deltaTime)
                {
                    // (Perform fixed update)

                    accumulator     -= Time.fixed.deltaTime;
                    Time.fixed.time += Time.fixed.deltaTime;
                    Time.tickCount++;
                }

                // Amount to lerp between physics steps
                double alpha = accumulator / Time.fixed.deltaTime;
                
                window->PreUpdate();
                render->BeginFrame();
                
                renderPipeline.RenderFrame(*render);

                render->EndFrame();
                window->Update();

                Time.frameCount++;
            }
        }

        void Shutdown()
        {
            render->Shutdown();
            delete render;
            delete window;
            Window::Shutdown();
        }
    };

    inline class Engine Engine;

}