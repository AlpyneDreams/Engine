#pragma once

#include "common/Common.h"
#include "render/Render.h"

#include <cstddef>

namespace engine
{
    struct IndexBuffer {
        enum { UInt16, UInt32 } type = UInt32;
        uint indexCount = 0;
        const void* indices;
        render::Handle* handle;

        IndexBuffer() {}

        IndexBuffer(const uint32* indices, size_t size)
          : type(UInt32),
            indexCount(size / sizeof(uint32)),
            indices(indices)
        {}

        IndexBuffer(const uint16* indices, size_t size)
          : type(UInt16),
            indexCount(size / sizeof(uint16)),
            indices(indices)
        {}

        inline size_t IndexSize() const { return type == UInt32 ? 4 : 2; }
        inline size_t Size() const {
            return indexCount * IndexSize();
        }
    };

}