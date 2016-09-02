#pragma once

#include "../iq.hpp"
#include "../pi.hpp"
#include "../sample.hpp"

#include <cmath>
#include <vector>

namespace dsp
{

template<bool Inverse>
struct GenericDynDFT
{
    GenericDynDFT(std::size_t size = 0) : size(size) {}

    std::size_t size;
    static constexpr bool inverse = Inverse;
};

using DynDFT = GenericDynDFT<false>;

using DynIDFT = GenericDynDFT<true>;


template<bool Inverse, typename Signal>
class DynDFTImpl;

template<bool Inverse, typename Signal>
DynDFTImpl<Inverse, Signal> transform(GenericDynDFT<Inverse> const& settings, Signal const& s) noexcept;

template<bool Inverse, typename Signal>
class DynDFTImpl
{
public:
    using sample_type = GenericIQ<typename Signal::value_type>;
    using value_type = typename Signal::value_type;

    using transform_type = GenericDynDFT<false>;
    using signal_type = Signal;

    [[gnu::pure]]
    sample_type operator()(Index m) const noexcept {
        constexpr value_type sign = Inverse ? 1 : -1;
        sample_type result{};

        for (Index n = 0; n < settings.size; ++n) {
            result += s(n) * exp(sign * generic_two_pi<value_type> * n * m / settings.size);
        }

        return Inverse ? (result / value_type(settings.size)) : result;
    }

private:
    friend DynDFTImpl transform<false, Signal>(transform_type const&, Signal const&) noexcept;

    constexpr DynDFTImpl(transform_type const& settings, signal_type const& s) noexcept
      : settings(settings), s(s)
      {}

    transform_type const settings;
    signal_type const s;
};

template<bool Inverse, typename Signal>
DynDFTImpl<Inverse, Signal> transform(GenericDynDFT<Inverse> const& settings, Signal const& s) noexcept {
    return { settings, s };
}

} /* namespace dsp */
