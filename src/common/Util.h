#pragma once

namespace engine
{
    template<typename T>
    concept Iterable = requires(T x) {
        x.begin(); x.end();
    };

    template <Iterable T>
    constexpr T TransformResult(auto first, auto last, auto op)
    {
        T result;
        std::transform(first, last, result.begin(), op);
        return result;
    }

    template <Iterable T>
    constexpr T TransformResult(auto first, auto last, auto otherFirst, auto op)
    {
        T result;
        std::transform(first, last, otherFirst, result.begin(), op);
        return result;
    }
}