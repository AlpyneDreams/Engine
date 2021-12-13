#include "math/Vector.h"

#include <iostream>
#include <cassert>

using namespace engine;

static_assert(sizeof(Vector<float, 1>) == sizeof(float)*1);
static_assert(sizeof(Vector<float, 2>) == sizeof(float)*2);
static_assert(sizeof(Vector<float, 3>) == sizeof(float)*3);
static_assert(sizeof(Vector<float, 4>) == sizeof(float)*4);

#define xassert(args...) assert((args))

template<class T>
void test() {
    const T values[4] = { T{0}, T{0}, T{0}, T{0} };
    const Vector<T,4> a(T{0}, T{1}, T{2}, T{3});
    const Vector<T,4> b(T{1});
    const Vector<T,4> c(values);
    const Vector<T,4> d(T{0}, T{4}, T{8}, T{12});
    
    xassert(a[0] == T(0));
    xassert(a[1] == T(1));
    xassert(a[2] == T(2));
    xassert(a[3] == T(3));

    // Operators
    xassert(a + b == Vector<T, 4>{ T(1), T(2), T(3), T(4) });
    xassert(a - b == Vector<T, 4>{ T{ -1 }, T(0), T(1), T(2) });
    xassert(a * b == a);
    xassert(a / b == a);
    xassert(-a == Vector<T, 4>{ T(0), T{ -1 }, T{ -2 }, T{ -3 } });
    xassert(-a != Vector<T, 4>{ T(1), T(0), T(5), T(1) });
    xassert(a * c == c);
    xassert(a * T(4) == Vector<T, 4>{ T(0), T(4), T(8), T{ 12 } });
    xassert(T(4) * a == d);
    xassert(d / T(4) == a);

    // Test dot/accumulate
    xassert(accumulate(a) == T(6));
    xassert(dot(a, d) == T{ 56 });

    // Test length squared
    xassert(lengthSqr(a) == T{ 14 });
    xassert(lengthSqr(b) == T(4));
    xassert(lengthSqr(c) == T(0));
    xassert(dot(a, a) == lengthSqr(a));

    const Vector<T,4> e{ T(2), T(2), T(2), T(2) };
    const Vector<T,4> f{ T(1) };
    const Vector<T,4> g{ T(6), T(0), T(0), T(0) };

    // Test length
    xassert(length(f) == T(2));
    xassert(length(g) == T(6));

    if constexpr (std::is_floating_point<T>::value)
    {
        // Normalize
        xassert(normalize(e) == Vector<T, 4>(T(0.5)));
        xassert(normalize(f) == Vector<T, 4>(T(0.5)));
        xassert(normalize(g) == Vector<T, 4>(T(1), T(0), T(0), T(0)));
    }
    else // Integers
    {
        const Vector<T, 4> a{ T(0), T(2), T(4), T(6) };
        const Vector<T, 4> b{ T(4), T(4), T(4), T(4) };
        const Vector<T, 4> c{ T(1), T(2), T(5), T(6) };

        // Test modulo
        xassert(a % b == Vector<T, 4>{ T(0), T(2), T(0), T(2) });
        xassert(T(4) % c == Vector<T, 4>{ T(0), T(0), T(4), T(4) });
    }


    // Printing
    std::cout << "a: " << a << std::endl;
    std::cout << "b: " << b << std::endl;
    std::cout << "c: " << c << std::endl;
    std::cout << "d: " << d << std::endl;


}

int main() {
    test<float>();
    test<double>();
    test<long double>();
    test<int8>();
    test<int16>();
    test<int32>();
    test<int64>();

    std::cout << "Tests passed!" << "\n";
    return 0;
}