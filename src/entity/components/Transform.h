#pragma once

#include "entity/Entity.h"
#include "math/Math.h"

namespace engine
{
    struct Transform : Component
    {
        Vector3     position;
        Quaternion  rotation;
        Vector3     scale;
    };
}