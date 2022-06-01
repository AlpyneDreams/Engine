#pragma once

namespace engine::editor
{
    // The global editor instance.
    extern inline class Editor Editor;

    class Editor
    {
    public:
        void Run();
    };
    
    inline class Editor Editor;
}