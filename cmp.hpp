#pragma once

#include <utility>

namespace dsp
{

template<typename T, typename U>
[[gnu::pure]]
inline constexpr T min(T const& a, U const& b) noexcept {
    return (a < b) ? a : b;
}

template<typename T, typename U>
[[gnu::pure]]
inline constexpr T max(T const& a, U const& b) noexcept {
    return (a > b) ? a : b;
}

template<typename T, typename U>
[[gnu::pure]]
inline constexpr std::pair<T, T> minmax(T const& a, U const& b) noexcept {
    return (a < b) ? std::pair<T, T>{ a, b } : std::pair<T, T>{ b, a };
}

template<typename T, typename U, typename V>
[[gnu::pure]]
inline constexpr T clamp(T const& value, U const& min, V const& max) noexcept {
    return dsp::min(dsp::max(value, min), max);
}

} /* namespace dsp */
