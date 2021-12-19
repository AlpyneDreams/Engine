#pragma once

#include "../RenderPipeline.h"

namespace engine::render
{
    // TODO: splat these members into ForwardRenderPipeline
    // unless using BGFX in-pipeline
    struct ForwardState;

    class ForwardRenderPipeline final : public RenderPipeline
    {
        ForwardState* state;
    public:
        ForwardRenderPipeline();
        virtual void Init(Render& r);
        virtual void RenderFrame(Render& r);
    };
}