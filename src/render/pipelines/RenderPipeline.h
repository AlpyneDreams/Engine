#pragma once

#include "../Render.h"
#include "engine/System.h"

namespace engine::render
{
    class RenderPipeline
    {
    protected:
        Render& r;
    public:
        RenderPipeline(Render& render) : r(render) {}

        virtual ~RenderPipeline() {}
        virtual void Init() = 0;
        virtual void RenderFrame() = 0;
    };
}