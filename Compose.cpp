#include <string>
#include <algorithm>
#include <vector>
#include <type_traits>
#include <iostream>

template <typename Head, typename... Func>
class Applier {
public:
    Applier(Head&& head, Func&&... func)
        : function(std::forward<Head>(head))
        , tail(std::forward<Func>(func)...)
    { }

    template <typename... Args>
    auto operator()(Args&&... args)
    {
        return function(
                tail.operator()(std::forward<Args>(args)...)
            );
    }
private:
    Head function;
    Applier<Func...> tail;
};

template<typename Head>
class Applier<Head> {
public:

    Applier(Head&& head)
        : function(std::forward<Head>(head))
    { }

    template <typename... Args>
    auto operator()(Args&&... args)
    {
        return function(
            std::forward<Args>(args)...
        );
    }

private:
    Head function;
};

template <typename... Func>
auto Compose(Func&&... func)
{
    return Applier<Func...>(std::forward<Func>(func)...);
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
    for (auto& i : d) {
        std::cout << i << " ";
    }
    std::cout << std::endl;
}
