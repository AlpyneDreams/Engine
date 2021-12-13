#pragma once

// Vector Macros (Implementation Details)

// Ex +, std::plus()
#define VECTOR_OPERATOR(op, fn)                                         \
    constexpr Vec operator op(const Vec& other) const {                 \
        return Transform(other, fn);                                    \
    }

// Ex: *
#define SCALAR_OPERATOR(op)                                             \
    constexpr Vec operator op(const T& scalar) const {                  \
        return Transform([scalar](T value) {return value op scalar;});  \
    }

// Ex: +=, std::plus()
#define VECTOR_OP_IN_PLACE(opeq, fn)                                   \
    constexpr Vec& operator opeq(const Vec& other) {                   \
        return TransformInPlace(fn);                                   \
    }

// Ex: *=, *
#define SCALAR_OP_IN_PLACE(opeq, op)                                           \
    constexpr Vec& operator opeq(const T& scalar) {                            \
        return TransformInPlace([scalar](T value) {return value op scalar;});  \
    }
