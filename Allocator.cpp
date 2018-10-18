#include <iostream>
#include <vector>

namespace dsh {

template <typename T, size_t N>
class static_memory {
public:
    static T* capture(size_t n) {
        if (m_is_free && n <= N) {
            m_is_free = false;
            return m_memory;
        } else {
            return nullptr;
        }
    }

    static void free() {
        m_is_free = true;
    }

private:
    static T m_memory[N];
    static bool m_is_free;
};

template <typename T, size_t N>
T static_memory<T, N>::m_memory[N];

template <typename T, size_t N>
bool static_memory<T, N>::m_is_free = true;

template<typename T>
class allocator {
public:
    using value_type = T;

    allocator() = default;

    value_type* allocate(size_t n) {

        value_type* pointer = static_alloc::capture(n);

        if (pointer == nullptr) {
             // static alloc failed
            std::cout << "Allocate memory in a heap. Size=" << n << std::endl;
            pointer = new value_type[n];
        } else {
            // static alloc success
            std::cout << "Allocate memory in a static memory. Size=" << n << std::endl;
            ptr_to_static = pointer;
        }

        if (pointer == nullptr) {
            std::cout << "Operation failed" << std::endl;
            throw std::bad_alloc{};
        } else {
            return pointer;
        }
    }

    void deallocate(value_type* p, size_t n) noexcept {
        if (p == ptr_to_static) {
            std::cout << "Deallocate a memory in static. Size=" << n << std::endl;
            static_alloc::free();
            ptr_to_static = nullptr;
        } else {
            std::cout << "Deallocate a memory in heap. Size=" << n << std::endl;
            operator delete[](p);
        }
    }

private:
    using static_alloc = static_memory<value_type, 20>;
    value_type* ptr_to_static = nullptr;

};
} // namespace dsh

int main() {
    std::vector<int, dsh::allocator<int>> vec;
    vec.push_back(1);
    std::vector<int, dsh::allocator<int>> vec2;
    vec2.push_back(2);
    std::vector<long, dsh::allocator<long>> vec3;
    vec3.push_back(4);
    return 0;
}

