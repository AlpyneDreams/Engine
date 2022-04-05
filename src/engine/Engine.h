#pragma once

#include "platform/Window.h"
#include "render/Render.h"
#include "render/pipelines/RenderPipeline.h"
#include "render/pipelines/forward/Forward.h"

#include "Time.h"
#include "System.h"

namespace engine
{
    // The global engine instance.
    extern inline class Engine Engine;
    
    class Engine
    {
    private:
        Window* window = Window::CreateWindow();
        render::Render* render = render::Render::Create();
        render::ForwardRenderPipeline renderPipeline = render::ForwardRenderPipeline(*render);

        SystemGroup systems;

    public:
        void Run()
        {
            Init();

            // Run the main loop
            Start();

            Shutdown();
        }

    protected:
        void Init()
        {
            window->Create("Engine", 1280, 720, true);
            render->Init(window);
            renderPipeline.Init();
        }

        void Start()
        {
            systems.Start();

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

                while (accumulator >= Time.fixed.deltaTime)
                {
                    // Perform fixed updates
                    systems.Tick();

                    accumulator     -= Time.fixed.deltaTime;
                    Time.fixed.time += Time.fixed.deltaTime;
                    Time.tickCount++;
                }

                // Amount to lerp between physics steps
                double alpha = accumulator / Time.fixed.deltaTime;
                
                // Process input
                window->PreUpdate();

                // Perform system updates
                systems.Update();

                // Render
                render->BeginFrame();
                renderPipeline.RenderFrame();
                render->EndFrame();

                // Present
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