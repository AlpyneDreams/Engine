#pragma once

#include "../Render.h"

namespace engine::render
{
    class RenderPipeline
    {
    public:
        virtual ~RenderPipeline() {}
        virtual void Init(Render& r) = 0;
        virtual void RenderFrame(Render& r) = 0;
    };
}