#pragma once

#include "cmp.hpp"
#include "pi.hpp"
#include "sample.hpp"

namespace dsp
{

template<typename Signal>
class FMDemodImpl;

class FMDemod
{
public:
    constexpr FMDemod(std::size_t dev, std::size_t fs) noexcept
        : dev(dev), fs(fs)
        {}

private:
    template<typename>
    friend class FMDemodImpl;

    template<typename T>
    constexpr T fac() const noexcept {
        return T(fs) / (generic_two_pi<T> * T(dev));
    }

    std::size_t dev;
    std::size_t fs;
};

class NBFMDemod : public FMDemod
{
public:
    constexpr NBFMDemod(std::size_t fs) noexcept
        : FMDemod(2500, fs)
        {}
};

class WBFMDemod : public FMDemod
{
public:
    constexpr WBFMDemod(std::size_t fs) noexcept
        : FMDemod(75000, fs)
        {}
};


template<typename Signal>
constexpr FMDemodImpl<Signal> demod(FMDemod const& settings, Signal const& s) noexcept;

template<typename Signal>
class FMDemodImpl
{
public:
    using sample_type = typename Signal::value_type;
    using value_type = sample_type;

    using demod_type = FMDemod;
    using signal_type = Signal;

    [[gnu::pure]]
    constexpr sample_type operator()(Index n) const noexcept {
        return clamp(delta_phi(s(n), s(n-1), s(n-2)) * fac, -1.0f, 1.0f);
    }

private:
    friend FMDemodImpl demod<Signal>(demod_type const&, signal_type const&) noexcept;

    constexpr FMDemodImpl(demod_type const& settings, signal_type const& s) noexcept
        : fac(settings.fac<value_type>()), s(s)
        {}

    value_type const fac;
    signal_type const s;
};

template<typename Signal>
inline constexpr FMDemodImpl<Signal> demod(FMDemod const& settings, Signal const& s) noexcept {
    return { settings, s };
}

} /* namespace dsp */
