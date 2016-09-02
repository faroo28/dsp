#pragma once

#include "pi.hpp"
#include "sample.hpp"

#include <cmath>
#include <array>

namespace dsp
{

template<typename T, std::size_t Size>
class GenericBlackmanWindow : private std::array<T, Size>
{
public:
    using sample_type = T;
    using value_type = sample_type;

    using window_type = GenericBlackmanWindow<T, Size>;
    static constexpr std::size_t size = Size;

    GenericBlackmanWindow() noexcept {
        for (Index n = 0; n < size; ++n)
            (*this)[n] = value(n);
    }

    [[gnu::pure]]
    constexpr sample_type operator()(Index n) const noexcept {
        return (*this)[n];
    }

    [[gnu::const]]
    static sample_type value(Index n) noexcept {
        T angle = generic_two_pi<T> / (size - 1) * n;
        return T(0.42) - T(0.5)*std::cos(angle) + T(0.08)*std::cos(2*angle);
    }
};

template<std::size_t Size>
using BlackmanWindow = GenericBlackmanWindow<float, Size>;

template<typename T, std::size_t Size>
GenericBlackmanWindow<T, Size> const& window(GenericBlackmanWindow<T, Size> const& settings) {
    return settings;
}

template<typename T, std::size_t Size>
GenericBlackmanWindow<T, Size>&& window(GenericBlackmanWindow<T, Size>&& settings) {
    return std::move(settings);
}

} /* namespace dsp */
