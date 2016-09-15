#pragma once

#include "cmp.hpp"
#include "iq.hpp"
#include "pi.hpp"
#include "sample.hpp"

namespace dsp
{

template<typename Signal>
class FMMod;

template<typename Signal>
class FMDemod;

class FM
{
public:
    constexpr FM(std::size_t dev, std::size_t fs) noexcept
        : dev(dev), fs(fs)
        {}

private:
    template<typename>
    friend class FMMod;

    template<typename>
    friend class FMDemod;

    template<typename T>
    constexpr T fac() const noexcept {
        return T(dev) / T(fs) * generic_two_pi<T>;
    }

    std::size_t dev;
    std::size_t fs;
};

class NBFM : public FM
{
public:
    constexpr NBFM(std::size_t fs) noexcept
        : FM(2500, fs)
        {}
};

class WBFM : public FM
{
public:
    constexpr WBFM(std::size_t fs) noexcept
        : FM(75000, fs)
        {}
};

template<typename Signal>
constexpr FMMod<Signal> mod(FM const& settings, Signal const& s) noexcept;

template<typename Signal>
class FMMod
{
public:
    using sample_type = GenericIQ<typename Signal::value_type>;
    using value_type = typename Signal::value_type;

    using modulation_type = FM;
    using signal_type = Signal;

    constexpr sample_type operator()(Index n) const noexcept {
        accum += clamp(I(s(n)), -1.0f, 1.0f) * fac;
        return exp(accum);
    }

private:
    friend FMMod mod<Signal>(modulation_type const&, signal_type const&) noexcept;

    constexpr FMMod(modulation_type const& settings, signal_type const& s) noexcept
        : fac(J<sample_type>*settings.fac<value_type>()), s(s), accum()
        {}

    sample_type const fac;
    signal_type const s;

    sample_type mutable accum;
};

template<typename Signal>
inline constexpr FMMod<Signal> mod(FM const& settings, Signal const& s) noexcept {
    return { settings, s };
}


template<typename Signal>
constexpr FMDemod<Signal> demod(FM const& settings, Signal const& s) noexcept;

template<typename Signal>
class FMDemod
{
public:
    using sample_type = typename Signal::value_type;
    using value_type = sample_type;

    using modulation_type = FM;
    using signal_type = Signal;

    [[gnu::pure]]
    constexpr sample_type operator()(Index n) const noexcept {
        return clamp(delta_phi(s(n), s(n-1), s(n-2)) * fac, -1.0f, 1.0f);
    }

private:
    friend FMDemod demod<Signal>(modulation_type const&, signal_type const&) noexcept;

    constexpr FMDemod(modulation_type const& settings, signal_type const& s) noexcept
        : fac(1/settings.fac<value_type>()), s(s)
        {}

    value_type const fac;
    signal_type const s;
};

template<typename Signal>
inline constexpr FMDemod<Signal> demod(FM const& settings, Signal const& s) noexcept {
    return { settings, s };
}

} /* namespace dsp */
