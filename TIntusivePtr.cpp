#include <exception>
#include <stdexcept>

#include <iostream>

template <typename T>
class TRefCounter {
public:

    // TIntrusivePtr methods
    size_t UseCount() const {
        return m_use_counter;
    }

    size_t Decrement() {
        if (m_use_counter == 0) {
            throw std::logic_error{"Invalid decrement a counter"};
        }
        return --m_use_counter;
    }

    size_t Increment() {
        return ++m_use_counter;
    }

private:


    size_t m_use_counter = 0;

    template <class U>
    class TIntrusivePtr;

    friend class TIntrusivePtr<T>;
};

template<typename T> //enable if T is base of TRefCounter<T>
class TIntrusivePtr {
public:

    TIntrusivePtr(T* object)
        : m_object(object)
    {
        if (m_object) {
            m_object->Increment();
        }
    }

    ~TIntrusivePtr() noexcept {
        if (m_object != nullptr && m_object->Decrement() == 0) {
            delete m_object;
        }
    }

    T* operator ->() {
        if (m_object == nullptr) {
            throw std::runtime_error{"Access to nullptr"};
        }
        return m_object;
    }

    T& operator*() {
        if (m_object == nullptr) {
            throw std::runtime_error{"Access to nullptr"};
        }
        return *m_object;
    }

    friend bool operator==(
            const TIntrusivePtr<T>& lhs,
            const TIntrusivePtr<T>& rhs
        ) {
        return lhs.m_object == rhs.m_object;
    }

    friend bool operator!=(
            const TIntrusivePtr<T>& lhs,
            const TIntrusivePtr<T>& rhs
        ) {
        return !(lhs == rhs);
    }

    operator bool() const {
        return m_object != nullptr;
    }

    size_t UseCount() const {
        return m_object ? m_object->UseCount() : 0;
    }

    T* Get()
    { return m_object; }

    void Reset(T* object) {
        if (m_object != nullptr && m_object->Decrement == 0) {
            delete m_object;
        }

        m_object = object;
        if (m_object != nullptr) {
            m_object->Increment();
        }
    }

private:
    T* m_object = nullptr;
};

class TDoc : public TRefCounter<TDoc> {
public:
    using TPtr = TIntrusivePtr<TDoc>;
    ~TDoc() {
        std::cout << "Dtor" << std::endl;
    }
    int get() const {
        return answer;
    }

    int answer = 42;
};

struct assertion_failed : public std::logic_error {
    assertion_failed(const char* str)
        : std::logic_error(str)
    { }
};

#define ASSERT(expr) { \
    if (!(expr)) { \
        throw assertion_failed{#expr}; \
    } \
}

int main() {
    TDoc *p = new TDoc;
    TIntrusivePtr<TDoc> ip1(p);
    TIntrusivePtr<TDoc> ip2(p);
    ASSERT(ip1 == ip2)
    ASSERT(ip1.Get() == p)
    ASSERT(ip1.UseCount() == 2)
    {
        TIntrusivePtr<TDoc> ip3(p);
        ASSERT(ip3.UseCount() == 3)
        ASSERT(ip2.UseCount() == 3)
    }
    ASSERT(ip2.UseCount() == 2)
    TIntrusivePtr<TDoc> null_intrusive(nullptr);
    ASSERT(!null_intrusive)
    ASSERT(ip1 != null_intrusive)
    ASSERT(ip1->get() == 42);
    ASSERT((*ip2).answer == 42);
    return 0;
}
