#pragma once

#include "common/Filesystem.h"
#include "common/String.h"

#include "math/Math.h"

#include "tiny_obj_loader.h"

namespace engine
{
    struct Vertex_XYZ_RGB_UV {
        Vector3 pos;
        Vector3 color;
        Vector2 uv;

        // ugh
        bool operator==(const Vertex_XYZ_RGB_UV& other) const {
            using namespace hlslpp;
            return any(pos == other.pos) && any(color == other.color) && any(uv == other.uv);
        }
    };


    struct ModelOBJ {
        std::vector<Vertex_XYZ_RGB_UV> vertices;
        std::vector<uint32>            indices;
    };

    ModelOBJ LoadModelOBJ(const char* path);
}
