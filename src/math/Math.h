#pragma once

namespace engine
{

    template <typename T, size_t N>
    struct Vector;

    template <typename T, size_t M, size_t N>
    struct Matrix;

    // TODO: Move more stuff into this namespace.
    // Perhaps fns such as cross, dot, etc.
    namespace math
    {
        using float2 = Vector<float, 2>;
        using float3 = Vector<float, 3>;
        using float4 = Vector<float, 4>;

        using float4x4 = Matrix<float, 4, 4>;
    }
}