#include <iostream>
#include "function.hpp"
#include <functional>

using namespace vstl;

struct A {
    int foo() {
        return 5;
    }
};

int bar(int a) {
    return a * 2;
}

int main() {
    A a;

    std::function<int(const A&)> func = &A::foo;

    //Function<int, A*> func(A::foo);
    //std::cout << func(&a) << "\n";

    return 0;
}
