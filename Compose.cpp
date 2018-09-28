#include <string>
#include <algorithm>
#include <vector>
#include <type_traits>

template <typename Head, typename... Funcs>
class Applier {
public:
    Applier(Head&& head, Func&& func)
        : funciton(std::forward<Head>(head))
        , tail(std::forward<Func>(func)...)
    { }

    template <typename... Args>
    auto operator()(Args&& args)
    {
        return function(
                tail.apply(std::forward<Args>(args)...)
            );
    }
private:
    Head function;
    Applier<Func...> tail;
}

template<typename Head>
class Applier {
public:

    Applier(Head&& head)
        : function(std::forward<Head>(head))
    { }

    template <typename... Args>
    auto operator()(Args&& args)
    {
        return funciton(
            std::forward<Args>(args)
        );
    }

private:
    Head function;
};

template <typename... Func>
class Compositor {
public:
    using result_type = std::result_of_t<
        Last<Func...>::value
    >;

    template<typename... Args>
    result_type operator()(Args&& ... args) {
        return apply(std::forward<Args>(args)...);
    }
};

template <typename... Func>
auto Compose(func&& func)
{
    class Compositor {
    public:
        template <typename...
        auto operator()(
    };
};

const char* f2(const std::string& str) {
    return str.c_str();
}
int main() {
    std::string s[] = {"1.2", "2.343", "3.2"};
    std::vector<double> d(3);
    auto f1 = atof;
    std::transform(s,
                   s + 3,
                   d.begin(),
                   Compose(f1, f2));
    //...
}
