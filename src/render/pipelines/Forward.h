#pragma once

#include "RenderPipeline.h"
#include "core/Mesh.h"

namespace engine::render
{
    class ForwardRenderPipeline final : public RenderPipeline
    {
        Shader* shader;
        Mesh cube;

    public:
        using RenderPipeline::RenderPipeline;

        void Init() final override;
        void Render(RenderContext& ctx) final override;
    };
}