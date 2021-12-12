#pragma once

#include "platform/Window.h"

namespace engine
{
    struct RenderState;

    class Render
    {
    protected:
        RenderState* state;
    public:
        Render();
        void Init(Window* window);
        void Update();
        void Shutdown();
    };
}