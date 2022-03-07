#pragma once

#define HLSLPP_FEATURE_TRANSFORM
#include "hlsl++.h"

using Vector2   = hlslpp::float2;
using Vector3   = hlslpp::float3;
using Vector4   = hlslpp::float4;
using Matrix4x4 = hlslpp::float4x4;

/// Hash Functions ///

constexpr size_t hash_combine(size_t& seed, size_t hash) {
    // Based on GLM's implementation.
    hash += 0x9e3779b9 + (seed << 6) + (seed >> 2);
    return (seed ^= hash);
}

template<typename T>
struct hash_base {
    size_t seed = 0;
    std::hash<float> hasher;
};

namespace std {

    template<>
    struct hash<Vector2> : hash_base<float> {
        size_t operator()(Vector2 const& v) {
            hash_combine(seed, hasher(v.x));
            hash_combine(seed, hasher(v.y));
            return seed;
        }
    };

    template<>
    struct hash<Vector3> : protected hash<Vector2> {
        size_t operator()(Vector3 const& v) {
            hash<Vector2>::operator()(v.xy);
            hash_combine(seed, hasher(v.z));
            return seed;
        }
    };
}