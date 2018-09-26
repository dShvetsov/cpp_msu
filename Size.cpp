#include <cstddef>
#include <iostream>

template <class T, size_t N>
constexpr size_t Size(T(&)[N])
{ return N; }

int main() {
    int a[5];
    double b[Size(a)];
    std::cout << "Size of array b is " << Size(b) << std::endl;
}

