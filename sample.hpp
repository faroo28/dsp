#pragma once

#include <cstdlib>
#include <cmath>
#include <complex>

namespace dsp
{

using Index = std::ptrdiff_t;

template<typename T>
struct value_type
{
    using type = T;
};

template<typename T>
using value_type_t = typename value_type<T>::type;

template<typename T>
constexpr T J;

template<typename T>
constexpr std::complex<T> J<std::complex<T>>{ 0, 1 };

template<typename T>
[[gnu::pure]]
inline constexpr value_type_t<T> I(T const& s) noexcept {
    using namespace std;
    return real(s);
}

template<typename T>
[[gnu::pure]]
inline constexpr value_type_t<T> Q(T const& s) noexcept {
    using namespace std;
    return imag(s);
}

template<typename T>
[[gnu::pure]]
inline constexpr value_type_t<T> power(T const& s) noexcept {
    using namespace std;
    return norm(s);
}

template<typename T>
[[gnu::pure]]
inline constexpr value_type_t<T> mag(T const& s) noexcept {
    using namespace std;
    return abs(s);
}

template<typename T>
[[gnu::pure]]
inline constexpr value_type_t<T> amp(T const& s) noexcept {
    return mag(s);
}

template<typename T>
[[gnu::pure]]
inline constexpr value_type_t<T> phi(T const& s) noexcept {
    using namespace std;
    return arg(s);
}

template<typename T>
[[gnu::pure]]
inline constexpr T exp(T const& s) noexcept {
    using namespace std;
    return exp(s);
}

template<typename T>
[[gnu::pure]]
inline constexpr value_type_t<T> delta_phi(T const& s, T const& prev, T const& prev2) {
    return value_type_t<T>{};
}

} /* namespace dsp */
