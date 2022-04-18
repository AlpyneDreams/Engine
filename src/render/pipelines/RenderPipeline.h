#pragma once

#include "../Render.h"
#include "engine/System.h"

namespace engine::render
{
    class RenderPipeline : public System
    {
    protected:
        Render& r;
    public:
        RenderPipeline(Render& render) : r(render) {}

        virtual ~RenderPipeline() {}
        virtual void Start() override = 0;
        virtual void Update() override = 0;
    };
}