#pragma once

#include <limits>
#include <algorithm>
#include <type_traits>

#include "common/Common.h"

namespace engine
{
    template <typename T = float>
    struct ColorRGBA
    {
        static constexpr T NormalMin = T(0);
        static constexpr T NormalMax = T{ std::is_floating_point<T>::value ? 1.0 : std::numeric_limits<T>::max()};

        union {
            T data[4];
            struct {
                T r, g, b, a;
            };
        };

        explicit ColorRGBA(T r, T g, T b, T a = NormalMax)
            : r(r), g(g), b(b), a(a) {}


        uint32 Pack()
        {
            using std::clamp;
            constexpr T packScale = T(255) / NormalMax;
            return    uint32( clamp(r * packScale, T{0}, T{255}) ) << 24u 
                    | uint32( clamp(g * packScale, T{0}, T{255}) ) << 16u 
                    | uint32( clamp(b * packScale, T{0}, T{255}) ) <<  8u 
                    | uint32( clamp(a * packScale, T{0}, T{255}) ) <<  0u ;
        }
    };

    using Color = ColorRGBA<float>;

    // List of common colors
    const struct {
        Color Black = Color(0.0, 0.0, 0.0, 1.0);
    } Colors;

}