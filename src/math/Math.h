#pragma once

#define GLM_FORCE_SWIZZLE

#include <glm/vec2.hpp>
#include <glm/vec3.hpp>
#include <glm/vec4.hpp>
#include <glm/mat4x4.hpp>
#include <glm/gtc/quaternion.hpp>

namespace engine
{
    using Vector2    = glm::vec2;
    using Vector3    = glm::vec3;
    using Vector4    = glm::vec4;
    using Quaternion = glm::quat;

    using Matrix4x4  = glm::mat4x4;

    enum struct Space {
        Global,
        Local
    };
}
