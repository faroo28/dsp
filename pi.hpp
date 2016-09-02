#pragma once

namespace dsp
{

template<typename T>
constexpr T generic_pi = T(3.14159265358979323846264338327950288419716939937510L);

template<typename T>
constexpr T generic_half_pi = generic_pi<T> / 2;

template<typename T>
constexpr T generic_two_pi = 2 * generic_pi<T>;

constexpr float pi = generic_pi<float>;
constexpr float half_pi = generic_half_pi<float>;
constexpr float two_pi = generic_two_pi<float>;

} /* namespace dsp */
