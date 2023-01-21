#pragma once

#include "editor/Tools.h"
#include "imgui/Window.h"

#include "hammer/VMF.h"

namespace engine::hammer
{
    inline class editor::Tools& Tools = editor::Tools;
    
    struct MapRender;
    
    inline class Hammer
    {
    public:
    // File I/O //
        // TODO: multiple open maps
        VMF map;
        void Open(const char* path);
    
    // Systems //
        MapRender* Renderer;
        
    // GUI //
        GUI::Window* viewport;

    // Hammer Engine Loop //

        void Run();

    } Hammer;
}
