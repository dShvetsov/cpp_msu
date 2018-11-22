#ifndef UTILS_H
#define UTILS

#include <vector>

#define CHECK_ARG(x) \
    if (! (x) ) { \
        throw std::invalid_argument(std::string("Argument must satisfy condition: ") + #x ); \
    } 

template<class T>
T sum(const std::vector<T>& v)
{
    T result = 0;
    for (const auto& i : v) {
        result += i;
    }
    return result;
}

double log(double base, double x) {
    return std::log(x) / std::log(base);
}

bool equal(double lhs, double rhs, double eps = 0.000000000000000001) {
    return std::abs(lhs - rhs) < eps;
}

#endif
