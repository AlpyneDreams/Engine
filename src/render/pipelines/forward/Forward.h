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
        using RenderPipeline::RenderPipeline;

        void Start() final override;
        void Update() final override;
    };
}