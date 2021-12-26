#pragma once

#include "platform/Window.h"
#include "math/Math.h"
#include "math/Color.h"
#include "render/core/VertexBuffer.h"

namespace engine { class Engine; }

namespace engine::render
{
    struct IndexBuffer;

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

        //virtual VertexBuffer* CreateVertexBuffer(void* data, VertexLayout& layout);
        virtual IndexBuffer* CreateIndexBuffer(const void* data, size_t size);

        virtual void SetIndexBuffer(IndexBuffer* buffer);

        virtual float GetAspectRatio() = 0;

        virtual void SetViewTransform(hlslpp::float4x4& view, hlslpp::float4x4& proj) = 0;
        virtual void SetTransform(hlslpp::float4x4& matrix) = 0;

        virtual void SetClearColor(bool clear, Color color = Colors.Black) = 0;
        virtual void SetClearDepth(bool clear, float depth = 1.0f) = 0;
    };
}