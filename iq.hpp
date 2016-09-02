#pragma once

#include <cmath>
#include <complex>

#include "cmp.hpp"
#include "sample.hpp"

namespace dsp
{

template<typename T>
struct GenericIQ
{
    using value_type = T;

    constexpr GenericIQ() noexcept = default;

    template<typename U, typename V>
    constexpr GenericIQ(U const& I, V const& Q) noexcept
        : I(I), Q(Q)
        {}

    template<typename U>
    constexpr GenericIQ(U const& s) noexcept
        : I(s)
        {}

    template<typename U>
    constexpr GenericIQ(std::complex<U> const& s) noexcept
        : I(s.real()), Q(s.imag())
        {}

    template<typename U>
    constexpr GenericIQ(GenericIQ<U> const& s) noexcept
        : I(s.I), Q(s.Q)
        {}

    [[gnu::pure]]
    constexpr operator std::complex<T>() const noexcept {
        return { I, Q };
    }

    template<typename U>
    GenericIQ& operator=(U const& s) noexcept {
        I = s;
        Q = value_type{};
        return *this;
    }

    template<typename U>
    GenericIQ& operator=(std::complex<U> const& s) noexcept {
        I = s.real();
        Q = s.imag();
        return *this;
    }

    template<typename U>
    GenericIQ& operator=(GenericIQ<U> const& s) noexcept {
        I = s.I;
        Q = s.Q;
        return *this;
    }

    template<typename U>
    GenericIQ& operator+=(U const& s) noexcept
    {
        *this = *this + s;
        return *this;
    }

    template<typename U>
    GenericIQ& operator-=(U const& s) noexcept
    {
        *this = *this - s;
        return *this;
    }

    template<typename U>
    GenericIQ& operator*=(U const& s) noexcept
    {
        *this = *this * s;
        return *this;
    }

    template<typename U>
    GenericIQ& operator/=(U const& s) noexcept
    {
        *this = *this / s;
        return *this;
    }

    T I{};
    T Q{};
};

using IQ = GenericIQ<float>;

template<typename T>
struct value_type<GenericIQ<T>>
{
    using type = T;
};

template<typename T>
[[gnu::pure]]
inline constexpr T real(GenericIQ<T> const& s) noexcept {
    return s.I;
}

template<typename T>
[[gnu::pure]]
inline constexpr T imag(GenericIQ<T> const& s) noexcept {
    return s.Q;
}

template<typename T>
[[gnu::pure]]
inline constexpr GenericIQ<T> conj(GenericIQ<T> const& s) noexcept {
    return { s.I, -s.Q };
}

template<typename T>
[[gnu::pure]]
inline constexpr GenericIQ<T> proj(GenericIQ<T> const& s) noexcept {
    return std::proj(s);
}

template<typename T>
[[gnu::pure]]
inline constexpr T abs(GenericIQ<T> const& s) noexcept {
    return std::hypot(I(s), Q(s));
}

template<typename T>
[[gnu::pure]]
inline constexpr T arg(GenericIQ<T> const& s) noexcept {
    return std::atan2(Q(s), I(s));
}

template<typename T>
[[gnu::pure]]
inline constexpr T norm(GenericIQ<T> const& s) noexcept {
    return s.I*s.I + s.Q*s.Q;
}

template<typename T>
[[gnu::pure]]
inline constexpr GenericIQ<T> exp(GenericIQ<T> const& s) noexcept {
    return std::exp(s);
}


template<typename T>
[[gnu::pure]]
inline constexpr GenericIQ<T> const& operator+(GenericIQ<T> const& s) noexcept {
    return s;
}

template<typename T>
[[gnu::pure]]
inline constexpr GenericIQ<T> operator-(GenericIQ<T> const& s) noexcept {
    return { -s.I, -s.Q };
}

template<typename T, typename U>
[[gnu::pure]]
inline constexpr GenericIQ<T> operator+(GenericIQ<T> const& lhs, U const& rhs) noexcept {
    return { lhs.I+rhs, lhs.Q };
}

template<typename T, typename U>
[[gnu::pure]]
inline constexpr GenericIQ<U> operator+(T const& lhs, GenericIQ<U> const& rhs) noexcept {
    return { lhs+rhs.I, rhs.Q };
}

template<typename T, typename U>
[[gnu::pure]]
inline constexpr GenericIQ<T> operator+(GenericIQ<T> const& lhs, GenericIQ<U> const& rhs) noexcept {
    return { lhs.I+rhs.I, lhs.Q+rhs.Q };
}

template<typename T, typename U>
[[gnu::pure]]
inline constexpr GenericIQ<T> operator-(GenericIQ<T> const& lhs, U const& rhs) noexcept {
    return { lhs.I-rhs, lhs.Q };
}

template<typename T, typename U>
[[gnu::pure]]
inline constexpr GenericIQ<U> operator-(T const& lhs, GenericIQ<U> const& rhs) noexcept {
    return { lhs-rhs.I, -rhs.Q };
}

template<typename T, typename U>
[[gnu::pure]]
inline constexpr GenericIQ<T> operator-(GenericIQ<T> const& lhs, GenericIQ<U> const& rhs) noexcept {
    return { lhs.I-rhs.I, lhs.Q-rhs.Q };
}

template<typename T, typename U>
[[gnu::pure]]
inline constexpr GenericIQ<T> operator*(GenericIQ<T> const& lhs, U const& rhs) noexcept {
    return { lhs.I*rhs, lhs.Q*rhs };
}

template<typename T, typename U>
[[gnu::pure]]
inline constexpr GenericIQ<U> operator*(T const& lhs, GenericIQ<U> const& rhs) noexcept {
    return { lhs*rhs.I, lhs*rhs.Q };
}

template<typename T, typename U>
[[gnu::pure]]
inline constexpr GenericIQ<T> operator*(GenericIQ<T> const& lhs, GenericIQ<U> const& rhs) noexcept {
    auto const ac = lhs.I * rhs.I;
    auto const bd = lhs.Q * rhs.Q;
    auto const abcd = (lhs.I + lhs.Q) * (rhs.I + rhs.Q);

    return { ac - bd,  abcd - ac - bd };
}

template<typename T, typename U>
[[gnu::pure]]
inline constexpr GenericIQ<T> operator/(GenericIQ<T> const& lhs, U const& rhs) noexcept {
    return { lhs.I/rhs, lhs.Q/rhs };
}

template<typename T, typename U>
[[gnu::pure]]
inline constexpr GenericIQ<U> operator/(T const& lhs, GenericIQ<U> const& rhs) noexcept {
    return GenericIQ<U>{lhs} / rhs;
}

template<typename T, typename U>
[[gnu::pure]]
inline constexpr GenericIQ<T> operator/(GenericIQ<T> const& lhs, GenericIQ<U> const& rhs) noexcept {
    return lhs * conj(rhs) / norm(rhs);
}


template<typename T, typename U>
[[gnu::pure]]
inline constexpr bool operator==(GenericIQ<T> const& lhs, U const& rhs) noexcept {
    return lhs.I == rhs && lhs.Q == T{};
}

template<typename T, typename U>
[[gnu::pure]]
inline constexpr bool operator==(T const& lhs, GenericIQ<U> const& rhs) noexcept {
    return lhs == rhs.I && U{} == rhs.Q;
}

template<typename T, typename U>
[[gnu::pure]]
inline constexpr bool operator==(GenericIQ<T> const& lhs, GenericIQ<U> const& rhs) noexcept {
    return lhs.I == rhs.I && lhs.Q == rhs.Q;
}

template<typename T, typename U>
[[gnu::pure]]
inline constexpr bool operator!=(GenericIQ<T> const& lhs, U const& rhs) noexcept {
    return lhs.I != rhs || lhs.Q != T{};
}

template<typename T, typename U>
[[gnu::pure]]
inline constexpr bool operator!=(T const& lhs, GenericIQ<U> const& rhs) noexcept {
    return lhs != rhs.I || U{} != rhs.Q;
}

template<typename T, typename U>
[[gnu::pure]]
inline constexpr bool operator!=(GenericIQ<T> const& lhs, GenericIQ<U> const& rhs) noexcept {
    return lhs.I != rhs.I || lhs.Q != rhs.Q;
}


template<typename T, typename U>
[[gnu::pure]]
inline constexpr bool operator<(GenericIQ<T> const& lhs, U const& rhs) noexcept {
    return abs(lhs) < abs(rhs);
}

template<typename T, typename U>
[[gnu::pure]]
inline constexpr bool operator<(T const& lhs, GenericIQ<U> const& rhs) noexcept {
    return abs(lhs) < abs(rhs);
}

template<typename T, typename U>
[[gnu::pure]]
inline constexpr bool operator<(GenericIQ<T> const& lhs, GenericIQ<U> const& rhs) noexcept {
    return abs(lhs) < abs(rhs);
}

template<typename T, typename U>
[[gnu::pure]]
inline constexpr bool operator<=(GenericIQ<T> const& lhs, U const& rhs) noexcept {
    return abs(lhs) <= abs(rhs);
}

template<typename T, typename U>
[[gnu::pure]]
inline constexpr bool operator<=(T const& lhs, GenericIQ<U> const& rhs) noexcept {
    return abs(lhs) <= abs(rhs);
}

template<typename T, typename U>
[[gnu::pure]]
inline constexpr bool operator<=(GenericIQ<T> const& lhs, GenericIQ<U> const& rhs) noexcept {
    return abs(lhs) <= abs(rhs);
}

template<typename T, typename U>
[[gnu::pure]]
inline constexpr bool operator>(GenericIQ<T> const& lhs, U const& rhs) noexcept {
    return abs(lhs) > abs(rhs);
}

template<typename T, typename U>
[[gnu::pure]]
inline constexpr bool operator>(T const& lhs, GenericIQ<U> const& rhs) noexcept {
    return abs(lhs) > abs(rhs);
}

template<typename T, typename U>
[[gnu::pure]]
inline constexpr bool operator>(GenericIQ<T> const& lhs, GenericIQ<U> const& rhs) noexcept {
    return abs(lhs) > abs(rhs);
}

template<typename T, typename U>
[[gnu::pure]]
inline constexpr bool operator>=(GenericIQ<T> const& lhs, U const& rhs) noexcept {
    return abs(lhs) >= abs(rhs);
}

template<typename T, typename U>
[[gnu::pure]]
inline constexpr bool operator>=(T const& lhs, GenericIQ<U> const& rhs) noexcept {
    return abs(lhs) >= abs(rhs);
}

template<typename T, typename U>
[[gnu::pure]]
inline constexpr bool operator>=(GenericIQ<T> const& lhs, GenericIQ<U> const& rhs) noexcept {
    return abs(lhs) >= abs(rhs);
}


template<typename T, typename U, typename V>
[[gnu::pure]]
inline constexpr T fast_mag(GenericIQ<T> const& s, U const& alpha = 1, V const& beta = 1) noexcept {
    using namespace std;
    auto sorted = dsp::minmax(abs(I(s)), abs(Q(s)));
    return alpha*sorted.first + beta*sorted.second;
}

template<typename T>
[[gnu::pure]]
inline constexpr T delta_phi(GenericIQ<T> const& s, GenericIQ<T> const& prev, GenericIQ<T> const& prev2) noexcept {
    auto diff = s - prev2;
    return (I(prev)*Q(diff) - Q(prev)*I(diff)) / norm(s);
}

} /* namespace dsp */
