#pragma once
#include "platform/Platform.h"

#include <limits>
#include <algorithm>
#include <type_traits>

#include <raylib.h>

#include "common/Common.h"

namespace engine
{
    using RaylibColor = ::Color;
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

        inline operator RaylibColor() const {
            return RaylibColor {r, g, b, a};
        }

        inline operator ColorRGBA<byte>() const {
            return ColorRGBA<byte>(byte(r * 255), byte(g * 255), byte(b * 255), byte(a * 255));
        }
    };

    using Color  = ColorRGBA<byte>;
    using Colorf = ColorRGBA<float>;

    // List of common colors
    const struct {
        Color Black = Color(0, 0, 0, 255);
    } Colors;

}