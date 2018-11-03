#include <type_traits>
#include <cstdint>

template<class T, class U, class... Others>
struct are_same : std::false_type { };

template <class T, class U, class... Others>
struct are_same<T, T, U, Others...> : are_same<T, U, Others...> { };

template <class T, class U>
struct are_same<T, U> : std::is_same<T, U> {};

template <class... Args>
constexpr bool are_same_v = are_same<Args...>::value;

static_assert(are_same_v<int, int32_t, signed int>, "compile assert");
static_assert(!are_same_v<long, int32_t, signed int>, "compile assert");

int main() {
    return 0;
}
