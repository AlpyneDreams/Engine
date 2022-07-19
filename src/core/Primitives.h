#pragma once

#include "core/Mesh.h"
#include "assets/Asset.h"

namespace engine
{
    inline struct Primitives
    {
        static inline Mesh Cube = *LoadAsset<Mesh, ".OBJ">("models/cube.obj");
        static inline Mesh Teapot  = *LoadAsset<Mesh, ".OBJ">("models/teapot.obj");
    } Primitives;
}