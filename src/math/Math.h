#pragma once

#define GLM_FORCE_SWIZZLE

#include <glm/vec2.hpp>
#include <glm/vec3.hpp>
#include <glm/vec4.hpp>
#include <glm/mat4x4.hpp>
#include <glm/gtc/quaternion.hpp>

#include <glm/gtx/string_cast.hpp>

#include <ostream>

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

namespace glm
{
    // Automatic GLM string casts.
    // fmtlib requires these be in namespace glm

    // Write vecN to stream
    template <int N, typename T, glm::qualifier P>
    inline std::ostream& operator<<(std::ostream& out, const glm::vec<N, T, P>& g) {
        return out << glm::to_string(g);
    }

    // Write matNxN to stream
    template <int N, int M, typename T, glm::qualifier P>
    inline std::ostream& operator<<(std::ostream& out, const glm::mat<N, M, T, P>& g) {
        return out << glm::to_string(g);
    }

    // Write matNxN to stream
    inline std::ostream& operator<<(std::ostream& out, const glm::quat& g) {
        return out << glm::to_string(g);
    }
}
