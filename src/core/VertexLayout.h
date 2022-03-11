#pragma once

#include "common/Common.h"

#include <cstddef>
#include <vector>
#include <utility>
#include <typeinfo>
#include <typeindex>

namespace engine
{
    struct VertexAttribute {
        enum Mode { Default, None, Position, Normal, Tangent, Bitangent, Color, Indices, Weight, TexCoord };

        uint size;
        uint dimension;
        std::type_index type;
        bool normalized = false;
        Mode mode = Default;
    };

    class VertexLayout
    {
    public:
        using Attribute = VertexAttribute;

        template<typename T>
        VertexLayout& Add(uint dimension, Attribute::Mode mode = Attribute::Default, bool normalized = false) {
            layout.push_back(Attribute{sizeof(T), dimension, std::type_index(typeid(T)), normalized, mode});
            stride += sizeof(T) * dimension;
            return *this;
        }

        VertexLayout& Skip(uint bytes) {
            layout.push_back(Attribute{1, bytes, std::type_index(typeid(nullptr_t)), false, Attribute::None});
            stride += bytes;
            return *this;
        }

        inline uint Stride() const { return stride; }

        inline const std::vector<Attribute>& Attributes() const { return layout; }
    private:
        uint stride = 0;
        std::vector<Attribute> layout;
    };

}