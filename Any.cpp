#include <memory>
#include <type_traits>
#include <exception>
#include <iostream>
#include <string>

class Any {
public:
    template <typename T>
    Any(T&& value)
        : m_value(new concrete_holder<T>{ std::forward<T>(value) })
    { }


    template<typename T>
    void set(T&& v) {
        m_value.reset(new concrete_holder<T>{std::forward<T>(v) });
    }

    template<typename T>
    T& get() {
        auto result = std::dynamic_pointer_cast<concrete_holder<T>>(m_value);
        if (result != nullptr) {
            return result->value;
        } else {
            throw std::bad_cast{};
        }
    }

private:

    struct type_holder {
        virtual ~type_holder() {};
    };

    template <typename U>
    struct concrete_holder : type_holder {
        concrete_holder(U&& value)
            : value(std::forward<U>(value))
        { }

        std::decay_t<U> value;
    };

    std::shared_ptr<type_holder> m_value;
};



int main(){
    Any a(5);
    Any b(std::string("Hello World"));
    std::cout << a.get<int>() << std::endl;
    std::cout << b.get<std::string>() << std::endl;
    return 0;
}
