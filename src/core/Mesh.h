#pragma once

#include "common/Common.h"
#include "math/Math.h"
#include "VertexLayout.h"
#include "VertexBuffer.h"
#include "IndexBuffer.h"

namespace engine
{
    struct Mesh
    {
        struct Group {
            VertexBuffer vertices;
            IndexBuffer indices;
        };

        VertexLayout layout;
        std::vector<Group> groups;

        Mesh() {}

        Group& AddGroup() {
            Group& group = groups.emplace_back();
            group.vertices.layout = &layout;
            return group;
        }
        
        Mesh(VertexLayout& layout, const void* vertices, size_t vSize, const uint* indices, size_t iSize)
            : layout(layout)
        {
            groups.push_back(Group {
                VertexBuffer(&layout, vertices, vSize),
                IndexBuffer(indices, iSize)
            });
        }

        Mesh(VertexLayout& layout, const void* vertices, size_t vSize, const uint16* indices, size_t iSize)
            : layout(layout)
        {
            groups.push_back(Group {
                VertexBuffer(&layout, vertices, vSize),
                IndexBuffer(indices, iSize)
            });
        }    
    };

    

}