#pragma once

#include "core/Mesh.h"

#include "tiny_obj_loader.h"

namespace engine
{
    Mesh* LoadMeshOBJ(const char* path);
}
