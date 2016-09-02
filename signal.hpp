#pragma once

#include "sample.hpp"

#include <cstdlib>
#include <ratio>
#include <type_traits>

namespace dsp
{

template<std::size_t Period, typename Signal>
class PeriodicSignal;

template<std::size_t Period, typename Signal>
constexpr PeriodicSignal<Period, Signal> periodic(Signal s) noexcept;

template<std::size_t Period, typename Signal>
class PeriodicSignal
{
public:
    using sample_type = typename Signal::sample_type;
    using value_type = typename Signal::value_type;

    using signal_type = Signal;

    [[gnu::pure]]
    constexpr sample_type operator()(Index n) const noexcept {
        return s((n >= 0) ? n % Period : (Period - 1 - (-(n+1) % Period)));
    }

private:
    friend PeriodicSignal periodic<Period, Signal>(signal_type) noexcept;

    constexpr PeriodicSignal(signal_type s) noexcept : s(s) {}

    signal_type const s;
};

template<std::size_t Period, typename Signal>
inline constexpr PeriodicSignal<Period, Signal> periodic(Signal s) noexcept {
    return { s };
}


template<std::size_t Limit, typename Signal>
class ZeroPaddedSignal;

template<std::size_t Limit, typename Signal>
constexpr ZeroPaddedSignal<Limit, Signal> zero_pad(Signal s) noexcept;

template<std::size_t Limit, typename Signal>
class ZeroPaddedSignal
{
public:
    using sample_type = typename Signal::sample_type;
    using value_type = typename Signal::value_type;

    using signal_type = Signal;

    [[gnu::pure]]
    constexpr sample_type operator()(Index n) const noexcept {
        return (n < 0 || n >= Limit) ? sample_type{} : s(n);
    }

private:
    friend ZeroPaddedSignal zero_pad<Limit, Signal>(signal_type) noexcept;

    constexpr ZeroPaddedSignal(signal_type s) noexcept : s(s) {}

    signal_type const s;
};

template<std::size_t Limit, typename Signal>
inline constexpr ZeroPaddedSignal<Limit, Signal> zero_pad(Signal s) noexcept {
    return { s };
}


template<Index Delay, typename Signal>
class DelayedSignal;

template<Index Delay, typename Signal>
constexpr std::enable_if_t<Delay != 0, DelayedSignal<Delay, Signal>> delay(Signal s) noexcept;

template<Index Delay, typename Signal>
class DelayedSignal
{
public:
    using sample_type = typename Signal::sample_type;
    using value_type = typename Signal::value_type;

    using signal_type = Signal;

    [[gnu::pure]]
    constexpr sample_type operator()(Index n) const noexcept {
        return s(n - Delay);
    }

private:
    friend DelayedSignal delay<Delay, Signal>(signal_type) noexcept;

    constexpr DelayedSignal(signal_type s) noexcept : s(s) {}

    signal_type const s;
};

template<Index Delay, typename Signal>
inline constexpr std::enable_if_t<Delay != 0, DelayedSignal<Delay, Signal>> delay(Signal s) noexcept {
    return { s };
}

template<Index Delay, typename Signal>
inline constexpr std::enable_if_t<Delay == 0, Signal> delay(Signal s) noexcept {
    return s;
}


namespace detail
{
    template<size_t P, size_t Q>
    static constexpr size_t GCD = GCD<Q, P % Q>;

    template<size_t P>
    static constexpr size_t GCD<P, 0> = P;

    template<typename Ratio>
    using ratio_simplify = std::ratio<
        Ratio::num / GCD<Ratio::num, Ratio::den>,
        Ratio::den / GCD<Ratio::num, Ratio::den>>;

    enum class resample_mode
    {
        none,
        downsample,
        upsample
    };

    template<typename Ratio>
    struct resample_info
    {
        static_assert(!(Ratio::num < 0 || Ratio::den < 0),
                      "resample ratio must not contain negative numbers");
        static_assert(Ratio::num != 0, "resample factor must not be 0");
        static_assert(Ratio::den != 0, "resample factor divided by 0");

        using original_ratio = Ratio;
        using ratio = ratio_simplify<original_ratio>;

        static constexpr std::size_t N = ratio::num;
        static constexpr std::size_t D = ratio::den;

        static constexpr resample_mode mode =
            (N == D) ? resample_mode::none
                     : (N > D) ? resample_mode::upsample
                               : resample_mode::downsample;
    };
} /* namespace detail */


template<typename Info, typename Signal, detail::resample_mode = Info::mode>
class ResampledSignal;

template<typename Ratio, typename Signal>
constexpr std::enable_if_t<
    detail::resample_info<Ratio>::mode != detail::resample_mode::none,
    ResampledSignal<detail::resample_info<Ratio>, Signal>>
resample(Signal s) noexcept;

template<typename Info, typename Signal>
class ResampledSignal<Info, Signal, detail::resample_mode::downsample>
{
public:
    using sample_type = typename Signal::sample_type;
    using value_type = typename Signal::value_type;

    using signal_type = Signal;

    [[gnu::pure]]
    constexpr sample_type operator()(Index n) const noexcept {
        return s(n*Info::D / Info::N);
    }

private:
    friend ResampledSignal resample<
        typename Info::original_ratio, Signal>(signal_type) noexcept;

    constexpr ResampledSignal(signal_type s) noexcept : s(s) {}

    signal_type const s;
};

template<typename Info, typename Signal>
class ResampledSignal<Info, Signal, detail::resample_mode::upsample>
{
public:
    using sample_type = typename Signal::sample_type;
    using value_type = typename Signal::value_type;

    using signal_type = Signal;

    [[gnu::pure]]
    constexpr sample_type operator()(Index n) const noexcept {
        auto qr = std::div(n*Info::D, Info::N);
        return qr.rem ? 0 : s(qr.quot);
    }

private:
    friend ResampledSignal resample<
        typename Info::original_ratio, Signal>(signal_type) noexcept;

    constexpr ResampledSignal(signal_type s) noexcept : s(s) {}

    signal_type const s;
};

template<typename Ratio, typename Signal>
inline constexpr std::enable_if_t<
    detail::resample_info<Ratio>::mode != detail::resample_mode::none,
    ResampledSignal<detail::resample_info<Ratio>, Signal>>
resample(Signal s) noexcept {
    return { s };
}

template<typename Ratio, typename Signal>
inline constexpr std::enable_if_t<
    detail::resample_info<Ratio>::mode == detail::resample_mode::none, Signal>
resample(Signal s) noexcept {
    return s;
}

} /* namespace dsp */
