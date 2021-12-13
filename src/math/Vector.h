#pragma once

#include <algorithm>
#include <numeric>
#include <cmath>
#include <type_traits>
#include <iostream>
#include <typeinfo>

#include "common/Common.h"
#include "common/Util.h"

#include "Vector.inl"

namespace engine
{
    template <typename T, size_t N>
    struct Vector;

    using Vector2 = Vector<float, 2>;
    using Vector3 = Vector<float, 3>;
    using Vector4 = Vector<float, 4>;

    /** Vector<T, N> is a union of:
     *    T data[N];
     *    T x, y, z w;
     *    T r, g, b, a;
     * Components (.xyzw, .rgba) only exists if N is large enough.
     */
    template <typename T = float, size_t N = 4>
    struct Vector
    {
        using Vec = Vector<T, N>;
        
        static constexpr Vec Zero = Vec(0);
        static constexpr Vec One  = Vec(1);

        template <size_t I>
        static constexpr Vec Basis() requires ((I < N)) {
            Vec e(0.f);
            e[I] = 1.f;
            return e;
        }

        // TODO: Decide on an up-vector
        static constexpr Vector<T, 3> Up = Vector<T, 3>(0.f, 1.f, 0.f); 


        constexpr Vector() : data{} {}
        constexpr Vector(const Vec& other) = default;

        // Splat: Vector(1) = Vector(1, 1, ..., 1)
        constexpr explicit Vector(const T splat) {
            std::fill(begin(), end(), splat);
        }
        
        // Vector(data[N])
        constexpr Vector(const T components[N]) {
            std::copy(&components[0], &components[N], &data[0]);
        }
        
        template <typename... Args>
        constexpr Vector(const Args&... args) : data {args...} {
            static_assert(sizeof...(Args) == N);
        }

        constexpr const T& operator [](size_t i) const { return data[i]; }
        constexpr       T& operator [](size_t i)       { return data[i]; }

        constexpr operator T*() const { return &data[0]; }

        // Iteration
        constexpr const T* begin() const { return &data[0]; }
        constexpr       T* begin()       { return &data[0]; }
        constexpr const T* end()   const { return &data[N]; }
        constexpr       T* end()         { return &data[N]; }

        // Equality: ==, !=
        constexpr bool operator ==(const Vec& other) const {
            return std::equal(begin(), end(), other.begin());
        }
        constexpr bool operator !=(const Vec& other) const {
            return !operator ==(other);
        }


        // Transforms: Unary, Binary
        constexpr Vec Transform(auto unaryOp) const {
            return TransformResult<Vec>(begin(), end(), unaryOp);
        }
        constexpr Vec Transform(const T* other, auto binaryOp) const {
            return TransformResult<Vec>(begin(), end(), other, binaryOp);
        }
        constexpr Vec Transform(const Vec& other, auto binaryOp) const {
            return Transform(other.begin(), binaryOp);
        }

        // In-Place Transform: Unary, Binary
        constexpr Vec TransformInPlace(auto unaryOp) {
            std::transform(begin(), end(), unaryOp); return *this;
        }
        constexpr Vec TransformInPlace(const T* other, auto binaryOp) {
            std::transform(begin(), end(), other, begin(), binaryOp); return *this;
        }
        constexpr Vec TransformInPlace(const Vec& other, auto binaryOp) {
            return TransformInPlace(other.begin(), binaryOp);
        }
        
        /// OPERATORS ///

        constexpr Vec operator -() const {
            return Transform(std::negate());
        }

        VECTOR_OPERATOR(+, std::plus());
        VECTOR_OPERATOR(-, std::minus());
        VECTOR_OPERATOR(*, std::multiplies());      // Component (Hadamard) Product 
        VECTOR_OPERATOR(/, std::divides());
        VECTOR_OPERATOR(%, std::modulus());

        VECTOR_OP_IN_PLACE(+=, std::plus());
        VECTOR_OP_IN_PLACE(-=, std::minus());
        VECTOR_OP_IN_PLACE(*=, std::multiplies());  // Component (Hadamard) Product
        VECTOR_OP_IN_PLACE(/=, std::divides());
        VECTOR_OP_IN_PLACE(%=, std::modulus());

        SCALAR_OPERATOR(*);
        SCALAR_OPERATOR(/);
        SCALAR_OPERATOR(%);

        SCALAR_OP_IN_PLACE(*=, *);
        SCALAR_OP_IN_PLACE(/=, /);
        SCALAR_OP_IN_PLACE(%=, %);

    protected:
        // Nothing has size = 1, but a member of type Nothing
        // shouldn't add to the size of the class.
        typedef struct { } Nothing;

        // Padding<M> is used with a nameless bit-field to pad
        // space in the anonymous union structs.
        template <size_t M>
        static constexpr size_t Padding = (N >= M) ? sizeof(T) * (M-1) : 0;

        // Component<M> is accessible (type T) as long as the vector
        // has at least M components. Otherwise, it has type Nothing.
        template <size_t M>
        using Component = typename std::conditional<(N >= M), T, Nothing>::type;

        // Align to 16 if the size is a multiple of 4 and T is 4 bytes in size
        // to take advantage of aligned load/stores.
        static constexpr size_t Alignment = 
            (N % 4 == 0 && sizeof(T) == sizeof(uint32)) ? std::max<size_t>(16, alignof(T)) : alignof(T);

    public:
        union {
            alignas(Alignment) T data[N];

            struct {
                union { T x, r; };
            };
            struct {
                uint : Padding<1>;
                union { [[no_unique_address]] Component<1> y, g; };
            };
            struct {
                uint : Padding<2>;
                union { [[no_unique_address]] Component<2> z, b; };
            };
            struct {
                uint : Padding<3>;
                union { [[no_unique_address]] Component<3> w, a; };
            };
        };
    };

    // Binary Operators
    template <typename T, size_t N>
    constexpr Vector<T, N> operator*(T scalar, const Vector<T, N>& vector) {
        return vector.Transform([scalar](T value) { return scalar * value; });
    }
    template <typename T, size_t N>
    constexpr Vector<T, N> operator/(T scalar, const Vector<T, N>& vector) {
        return vector.Transform([scalar](T value) { return scalar / value; });
    }
    template <typename T, size_t N>
    constexpr Vector<T, N> operator%(T scalar, const Vector<T, N>& vector) {
        return vector.Transform([scalar](T value) { return scalar % value; });
    }

    template <typename T, size_t N>
    constexpr T accumulate(const Vector<T, N>& v, T base = T{}) {
        return std::accumulate(v.begin(), v.end(), base);
    }

    template <typename T, size_t N>
    constexpr T dot(const Vector<T, N>& a, const Vector<T, N>& b) {
        return accumulate(a * b); 
    }

    template <typename T, size_t N>
    constexpr T lengthSqr(const Vector<T, N>& v) {
        return dot(v, v);
    }

    template <typename T, size_t N>
    constexpr T length(const Vector<T, N>& v) {
        return std::sqrt(lengthSqr(v));
    }

    template <typename T, size_t N>
    constexpr Vector<T, N> normalize(const Vector<T, N>& v) {
        return v * T(T(1) / length(v));
    }

    // 3D-Specific

    // Cross-Product
    constexpr Vector3 cross(const Vector3& a, const Vector3& b) {
        return Vector3(
            a.y*b.z - a.z*b.y,
            a.z*b.x - a.x*b.z,
            a.x*b.y - a.y*b.x
        );
    }



    template <typename T, size_t N>
    std::ostream& operator<<(std::ostream& s, const Vector<T, N>& v) {
        s << "Vector<" << typeid(T).name() << ", " << N << ">(";
        for (size_t i = 0; i < N; i++) {
            s << v[i] << ((i != N-1) ? ", " : ")");
        }
        return s;
    }
}


// Hide macros from Vector.inl
#undef VECTOR_OPERATOR
#undef SCALAR_OPERATOR
#undef VECTOR_OP_IN_PLACE
#undef SCALAR_OP_IN_PLACE
