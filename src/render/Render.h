#pragma once

#include "platform/Window.h"
#include "math/Math.h"
#include "math/Color.h"

namespace engine { class Engine; }

namespace engine::render
{
    class Shader {};

    class Render
    {
        friend class ::engine::Engine;
    protected:
        virtual void Init(Window* window) = 0;
        virtual void BeginFrame() = 0;
        virtual void EndFrame() = 0;
        virtual void Shutdown() = 0;
    public:
        static Render* Create();
        virtual ~Render() {}

        virtual Shader* LoadShader(const char* vertexShader, const char* pixelShader) = 0; // TODO: Probably replace this
        virtual void    SetShader(Shader* shader) = 0;

        virtual void Submit() = 0;

        virtual float GetAspectRatio() = 0;

        virtual void SetViewTransform(hlslpp::float4x4& view, hlslpp::float4x4& proj) = 0;
        virtual void SetTransform(hlslpp::float4x4& matrix) = 0;

        virtual void SetClearColor(bool clear, Color color = Colors.Black) = 0;
        virtual void SetClearDepth(bool clear, float depth = 1.0f) = 0;
    };
}