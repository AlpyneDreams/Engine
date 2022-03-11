#pragma once

#include "common/Common.h"
#include "render/Render.h"
#include "VertexLayout.h"


namespace engine
{
    struct VertexBuffer {
        VertexLayout* layout;
        uint vertexCount = 0;
        union {
            const void* vertices;
            const float* vertexes;
        };
        render::Handle* handle;

        VertexBuffer() {}

        VertexBuffer(VertexLayout* layout, const void* vertices, size_t size)
          : layout(layout),
            vertexCount(size / layout->Stride()),
            vertices{vertices}
        {}

        inline size_t Size() const {
            if (layout == nullptr) {
                return vertexCount;
            } else {
                return vertexCount * layout->Stride();
            }
        }
    };


}