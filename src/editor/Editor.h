#pragma once

#include "entity/Entity.h"
#include "imgui/Window.h"
#include "render/Render.h"
#include "editor/Selection.h"

namespace engine::editor
{
    // The global editor instance.
    extern inline class Editor Editor;

    class Editor
    {
    public:
        Entity editorCamera;
        
        render::RenderTarget* rt_SceneView;
        
        // TODO: Support multiple instances of each
        GUI::Window* console;
        GUI::Window* outline;
        GUI::Window* inspector;
        GUI::Window* sceneView;

        Entity AddEntity()
        {
            Entity ent = World.CreateEntity();
            Selection.Select(ent);
            return ent;
        }

        void Run();
    };
    
    inline class Editor Editor;
}