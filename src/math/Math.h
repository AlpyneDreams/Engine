#pragma once

#define HLSLPP_FEATURE_TRANSFORM
#include "hlsl++.h"

namespace engine
{
    using Vector2    = hlslpp::float2;
    using Vector3    = hlslpp::float3;
    using Vector4    = hlslpp::float4;
    using Quaternion = hlslpp::quaternion;

    using Matrix4x4  = hlslpp::float4x4;
}
