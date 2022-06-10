#pragma once

#include "entity/Entity.h"
#include "imgui/Window.h"

namespace engine::editor
{
    // The global editor instance.
    extern inline class Editor Editor;

    class Editor
    {
    public:
        Entity editorCamera;
        
        // TODO: Support multiple instances of each
        GUI::Window* console;
        GUI::Window* outline;
        GUI::Window* inspector;

        void Run();
    };
    
    inline class Editor Editor;
}