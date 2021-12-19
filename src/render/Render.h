#pragma once

#include "platform/Window.h"
#include "math/Math.h"
#include "math/Color.h"

namespace engine { class Engine; }

namespace engine::render
{
    struct RenderState;

    class Render
    {
        friend class ::engine::Engine;
    protected:
        RenderState* state;

        Render();
        ~Render();
        void Init(Window* window);
        void BeginFrame();
        void EndFrame();
        void Shutdown();
    public:
        float GetAspectRatio();

        void SetViewTransform(hlslpp::float4x4& view, hlslpp::float4x4& proj);
        void SetTransform(hlslpp::float4x4& matrix);

        void SetClearColor(bool clear, Color color = Colors.Black);
        void SetClearDepth(bool clear, float depth = 1.0f);
    };
}