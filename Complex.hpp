#pragma once

namespace dsh {

template <typename T>
class Complex {
public:
    constexpr Complex(T real = 0, T imag = 0)
        : m_real(real), m_imag(imag)
    { }
    constexpr T GetRe() const
    { return m_real; }

    constexpr T GetIm() const
    { return m_imag; }

    constexpr void SetRe(T real)
    { m_real = real; }

    constexpr void SetIm(T imag)
    { m_imag = imag; }


private:
    T m_real;
    T m_imag;
};

template <typename T>
constexpr bool operator==(const Complex<T>& lhs,
                          const Complex<T>& rhs)
{
    return lhs.GetRe() == rhs.GetRe() && lhs.GetIm() == rhs.GetIm();
}

} // namespace dsh
