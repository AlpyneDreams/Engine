#pragma once

#include "core/Mesh.h"
#include "assets/Assets.h"

namespace engine
{
    inline struct Primitives
    {
        static inline Mesh Cube = *Assets.Load<Mesh, ".OBJ">("models/cube.obj");
        static inline Mesh Teapot  = *Assets.Load<Mesh, ".OBJ">("models/teapot.obj");
    } Primitives;
}