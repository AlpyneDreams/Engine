#pragma once

#include "common/Common.h"
#include "render/Render.h"
#include "GraphicsBuffer.h"

#include <cstddef>

namespace engine
{
    struct IndexBuffer : GraphicsBuffer
    {
        enum { UInt16, UInt32 } type = UInt32;
        const void* indices;

        IndexBuffer() {}

        IndexBuffer(const uint32* indices, size_t size)
          : GraphicsBuffer(size / sizeof(uint32)),
            indices(indices),
            type(UInt32)
        {}

        IndexBuffer(const uint16* indices, size_t size)
          : GraphicsBuffer(size / sizeof(uint16)),
            indices(indices),
            type(UInt16)
        {}

        inline size_t Stride() const override final {
          return type == UInt32 ? 4 : 2;
        }
    };

}