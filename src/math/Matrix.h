#pragma once

#include "Math.h"
#include "Vector.h"

#include <initializer_list>

namespace engine
{
    template <typename T, size_t M, size_t N>
    struct Matrix;

    using Matrix4x4 = Matrix<float, 4, 4>;

    // TODO: This is all kinds of fucked up

    template <typename T, size_t M, size_t N>
    struct Matrix
    {
        using Mtx = Matrix<T,M,N>;
        using Vec = Vector<T,N>;

        Vec data[M];

        constexpr Matrix() : data{} {}

        template <typename... Args>
        constexpr Matrix(const Args&... args) : data {args...} {
            static_assert(sizeof...(Args) == M);
        }

        constexpr Matrix(const T components[N][M]) {
            std::copy(&components[0], &components[N][M], &data[0]);
        }

        constexpr Matrix(const Matrix<T, M, N>& other) = default;

        constexpr const Vec& operator [](size_t i) const { return data[i]; }
        constexpr       Vec& operator [](size_t i)       { return data[i]; }

        constexpr operator T*() const { return const_cast<float*>(&data[0].data[0]); }


        static Matrix4x4 LookAt(Vector3 eye, Vector3 target, Vector3 globalUp = Vector3::Up)
        {
            using namespace math;

            const float3 view   = normalize(eye - target);

            const float3 uxv    = cross(globalUp, view);
            const float3 right  = normalize(uxv);
            const float3 up     = cross(view, right); 
            
            return Matrix4x4(
                float4 {right.x, up.x, view.x, -dot(right, eye)},
                float4 {right.y, up.y, view.y, -dot(up, eye)},
                float4 {right.z, up.z, view.z, -dot(view, eye)},
                float4 {0.f, 0.f, 0.f, 1.f}
            );
        }
    };
}