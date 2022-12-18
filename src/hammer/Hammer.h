#pragma once

#include "engine/Engine.h"
#include "editor/Editor.h"
#include "engine/System.h"
#include "imgui/Window.h"

#include "hammer/VMF.h"

namespace engine::hammer
{
    inline class editor::Editor& Editor = editor::Editor;
    
    inline class Hammer
    {
    public:
    // File I/O //
        // TODO: multiple open maps
        VMF map;
        void Open(const char* path);
        
    // GUI //
        GUI::Window* viewport;

    // Hammer Engine Loop //

        void Run();

    } Hammer;

    // TODO: Make this a RenderPipeline
    struct MapRender : public System
    {
        render::Render& r = Engine.Render;
        render::Shader* shader;

        void Start() final override;
        void Update() final override;
        void DrawEntity(MapEntity& entity);
    };
}
