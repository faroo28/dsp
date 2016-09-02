#pragma once

#include "../pi.hpp"
#include "../sample.hpp"

#include <cmath>
#include <vector>

namespace dsp
{

template<typename T>
class GenericDynBlackmanWindow : private std::vector<T>
{
public:
    using sample_type = T;
    using value_type = sample_type;

    using window_type = GenericDynBlackmanWindow<T>;

    GenericDynBlackmanWindow(std::size_t size)
        : std::vector<T>(size)
    {
        for (Index n = 0; n < size; ++n)
            (*this)[n] = value(size, n);
    }

    using std::vector<T>::size;

    [[gnu::pure]]
    sample_type operator()(Index n) const noexcept {
        return (*this)[n];
    }

    [[gnu::const]]
    static sample_type value(std::size_t size, Index n) noexcept {
        T angle = generic_two_pi<T> / (size - 1) * n;
        return T(0.42) - T(0.5)*std::cos(angle) + T(0.08)*std::cos(2*angle);
    }
};

using DynBlackmanWindow = GenericDynBlackmanWindow<float>;

template<typename T>
GenericDynBlackmanWindow<T> const& window(GenericDynBlackmanWindow<T> const& settings) {
    return settings;
}

template<typename T>
GenericDynBlackmanWindow<T>&& window(GenericDynBlackmanWindow<T>&& settings) {
    return std::move(settings);
}

} /* namespace dsp */
