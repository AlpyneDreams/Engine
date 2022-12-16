#pragma once

#include "editor/Editor.h"
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
}
