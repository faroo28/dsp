#pragma once

#include "iq.hpp"
#include "sample.hpp"

#include <array>

namespace dsp
{

template<typename T, std::size_t Size, std::size_t Delay = 1>
class GenericFIRFilter : public std::array<T, Size>
{
    using base = std::array<T, Size>;

public:
    using base::base;

    using tap_type = T;
    static constexpr std::size_t size = Size;
    static constexpr std::size_t delay = Delay;

    constexpr GenericFIRFilter(base const& data) noexcept
        : base(data)
        {}

    [[gnu::pure]]
    constexpr tap_type operator()(Index n) const noexcept
    {
        return (*this)[n];
    }
};

template<typename T, std::size_t Size, std::size_t Delay = 1>
using GenericIQFIRFilter = GenericFIRFilter<GenericIQ<T>, Size, Delay>;

template<std::size_t Size, std::size_t Delay = 1>
using IQFIRFilter = GenericIQFIRFilter<float, Size, Delay>;

template<std::size_t Size, std::size_t Delay = 1>
using RealFIRFilter = GenericFIRFilter<float, Size, Delay>;

template<std::size_t Size, std::size_t Delay = 1>
using FIRFilter = RealFIRFilter<Size, Delay>;


template<typename T, std::size_t Size, std::size_t Delay, typename Signal>
class FIRFilterImpl;

template<typename T, std::size_t Size, std::size_t Delay, typename Signal>
constexpr FIRFilterImpl<T, Size, Delay, Signal> filter(GenericFIRFilter<T, Size, Delay> const& settings, Signal const& s) noexcept;

template<typename T, std::size_t Size, std::size_t Delay, typename Signal>
class FIRFilterImpl
{
public:
    using sample_type = typename Signal::sample_type;
    using value_type = typename Signal::value_type;

    using filter_type = GenericFIRFilter<T, Size, Delay>;
    using signal_type = Signal;

    [[gnu::pure]] constexpr sample_type operator()(Index n) const noexcept;

private:
    friend FIRFilterImpl filter<T, Size, Delay, Signal>(filter_type const&, signal_type const&) noexcept;

    constexpr FIRFilterImpl(filter_type const& f, Signal const& s)
        : f(f), s(s)
        {}

    filter_type const f;
    signal_type const s;
};

template<typename T, std::size_t Size, std::size_t Delay, typename Signal>
[[gnu::pure]]
constexpr typename FIRFilterImpl<T, Size, Delay, Signal>::sample_type
FIRFilterImpl<T, Size, Delay, Signal>::operator()(Index n) const noexcept {
    sample_type data[filter_type::size] = {};

    // avoid backward memory access
    for (Index rn = filter_type::size - 1; rn >= 0; --rn)
        data[rn] = s(n - (rn * filter_type::delay));

    auto taps = f.data();
    sample_type result{};

    for (Index rn = 0; rn < filter_type::size; ++rn)
        result += data[rn] * taps[rn];

    return result;
}

template<typename T, std::size_t Size, std::size_t Delay, typename Signal>
inline constexpr FIRFilterImpl<T, Size, Delay, Signal> filter(GenericFIRFilter<T, Size, Delay> const& settings, Signal const& s) noexcept {
    return { settings, s };
}

} /* namespace dsp */
