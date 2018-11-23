#ifndef UTILS_H
#define UTILS

#include <cmath>
#include <ostream>
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

namespace terminal {

enum class settings : uint8_t {
    black           =  30,
    red             =  31,
    green           =  32,
    yellow          =  33,
    blue            =  34,
    magenta         =  35,
    cyan            =  36,
    white           =  37,
    reset           =   0,
    bold_bright     =   1,
    underline       =   4,
    inverse         =   7,
    bold_bright_off =  21,
    underline_off   =  24,
    inverse_off     =  27,
    blink           =   5,
};

class setup {
public:
    setup(std::initializer_list<settings> setts)
        : m_out(nullptr), m_settings(setts)
    { }
    ~setup() {
        if (m_out) {
            *m_out << "\033[0m";
        }
    }

    friend std::ostream& operator<<(std::ostream& out, setup&& s)
    {
        if (s.m_out != nullptr) {
           std::runtime_error("Try setup setting second time");
        }
        s.m_out = &out;
        out << "\033["; // start of manage commands
        for (auto i = s.m_settings.begin(); i != s.m_settings.end(); i++) {
            out << static_cast<uint32_t>(*i);
            // separate or end commands
            out << (i != s.m_settings.end() - 1 ? ';' : 'm');
        }
        return out;
    }
private:
    std::ostream* m_out;
    std::vector<settings> m_settings;
};

} // namespace terminal

#endif
