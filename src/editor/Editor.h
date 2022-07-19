#pragma once

#include "entity/Entity.h"
#include "imgui/Window.h"
#include "render/Render.h"
#include "editor/Selection.h"

namespace engine::editor
{
    inline class Editor
    {
    public:
        Entity editorCamera;
        
        render::Shader* sh_Wireframe;

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
    } Editor;
}