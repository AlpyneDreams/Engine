#pragma once

#include "../RenderPipeline.h"
#include "core/Mesh.h"

namespace engine::render
{
    class ForwardRenderPipeline final : public RenderPipeline
    {
        Shader* shader;
        Mesh cube;

    public:
        ForwardRenderPipeline() {}
        virtual void Init(Render& r);
        virtual void RenderFrame(Render& r);
    };
}