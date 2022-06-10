#pragma once

#include "common/Common.h"
#include "render/Render.h"
#include "GraphicsBuffer.h"
#include "VertexLayout.h"


namespace engine
{
    struct VertexBuffer : GraphicsBuffer
    {
        VertexLayout* layout;
        union {
            const void* pointer;
            const float* vertices;
        };

        VertexBuffer() {}

        VertexBuffer(VertexLayout* layout, const void* vertices, size_t size)
          : GraphicsBuffer(size / layout->Stride()),
            pointer(vertices),
            layout(layout)
        {}

        inline size_t Stride() const override final {
            return layout == nullptr ? 1 : layout->Stride();
        }
    };


}