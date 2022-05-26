#pragma once

#include "engine/Engine.h"
#include "editor/gui/Outline.h"
#include "editor/gui/Inspector.h"
#include "entity/Entity.h"

namespace engine::editor
{
    // The global editor instance.
    extern inline class Editor Editor;

    class Editor
    {
    public:        
        void Run()
        {
            Engine.systems.AddSystem<GUI::ConsoleWindow>();
            Engine.systems.AddSystem<editor::Outline>();
            Engine.systems.AddSystem<editor::Inspector>();

            Engine.Init();
            Engine.Loop();
            Engine.Shutdown();
        }
    };
    
    inline class Editor Editor;


}