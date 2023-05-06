#include <iostream>

#define printVar(x) std::cout << #x << " = " << x << std::endl

#include "dlvector.h"
#include "dlmatrix.h"

int main(int argc, const char* argv[])
{
    std::cout << "hello world" << std::endl;

    lithium::Vector u{1.0f, 2.0f, 3.0f};
    lithium::Vector v{4.0f, 5.0f, 6.0f};

    printVar(u);
    printVar(v);
    printVar(u * 2.0f);
    printVar(u + v);
    printVar(v * 2.0f);
    printVar(u * v);
    printVar(u.dot(v));

    assert(u == u);
    assert(u != v);
    assert(u + v == v + u);
    
    lithium::Vector u0{2.0f, 4.0f, 6.0f};
    assert(u * 2.0f == u0);
    u0 /= 2.0f;
    assert(u == u0);

    lithium::Vector u1{2.0f, 4.0f, 6.0f};
    assert(u * 2.0f == u1);
    u1 *= 0.5f;
    assert(u == u1);
    u1 += 1.0f;
    assert(u1 != v);
    v = u1;
    assert(v == u1);

    lithium::Matrix m1{{
        lithium::Vector{1.0f, 2.0f, 3.0f},
        lithium::Vector{4.0f, 5.0f, 6.0f},
        lithium::Vector{7.0f, 8.0f, 9.0f}
    }};

    printVar(m1);
    printVar(m1 * u);
    printVar(m1.dot(u));

    auto I = lithium::Matrix::identity(2);
    printVar(I);
    auto I2 = lithium::Matrix::identity(5, 2.3f);
    printVar(I2);

    return 0;
}