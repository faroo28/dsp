#pragma once

#include "../sample.hpp"

namespace dsp
{

template<typename Signal>
class DynPeriodicSignal;

template<typename Signal>
constexpr DynPeriodicSignal<Signal> periodic(std::size_t period, Signal const& s) noexcept;

template<typename Signal>
class DynPeriodicSignal
{
public:
    using sample_type = typename Signal::sample_type;
    using value_type = typename Signal::value_type;

    using signal_type = Signal;

    [[gnu::pure]]
    constexpr sample_type operator()(Index n) const noexcept {
        return s((n >= 0) ? n : (period + n));
    }

private:
    friend DynPeriodicSignal periodic<Signal>(std::size_t, signal_type const&) noexcept;

    constexpr DynPeriodicSignal(std::size_t period, signal_type const& s) noexcept
        : period(period), s(s)
        {}

    std::size_t const period;
    signal_type const s;
};

template<typename Signal>
inline constexpr DynPeriodicSignal<Signal> periodic(std::size_t period, Signal const& s) noexcept {
    return { period, s };
}

} /* namespace dsp */
