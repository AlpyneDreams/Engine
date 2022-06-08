#pragma once

#include "platform/Window.h"

#include "Time.h"
#include "System.h"
#include "render/RenderSystem.h"

#include "console/Console.h"
#include "imgui/ConsoleWindow.h"
#include "console/ConsoleCommands.h"


namespace engine
{
    // The global engine instance.
    extern inline class Engine Engine;

    // Forward declarations.
    namespace editor { class Editor; }
    
    class Engine
    {
        friend class editor::Editor;
    protected:
        Window* window      = Window::CreateWindow();
        RenderSystem render = RenderSystem(window);

        SystemGroup systems;

    public:
        void Run()
        {
            systems.AddSystem<GUI::ConsoleWindow>();
            
            Init();
            // Run the main loop
            Loop();
            Shutdown();
        }

    protected:
        void Init()
        {
            // (Load app info and configs)
            render.Start();
            // (Load important resources)
            // (Load main scenes)
        }

        bool ShouldQuit()
        {
            return window->ShouldClose(); 
        }

        void Loop()
        {
            systems.Start();

            Time::Seconds lastTime    = Time::GetTime();
            Time::Seconds accumulator = 0;

            while (!ShouldQuit())
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
                render.Update();

                // Present
                window->Update();

                Time.frameCount++;
            }
        }

        void Shutdown()
        {
            render.Shutdown();
            delete window;
            Window::Shutdown();
        }
    };

    inline class Engine Engine;

}