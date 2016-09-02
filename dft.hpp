#pragma once

#include "iq.hpp"
#include "pi.hpp"
#include "sample.hpp"

#include <cmath>
#include <array>

namespace dsp
{

template<std::size_t Size, bool Inverse>
struct GenericDFT
{
    static constexpr std::size_t size = Size;
    static constexpr bool inverse = Inverse;
};

template<std::size_t Size>
using DFT = GenericDFT<Size, false>;

template<std::size_t Size>
using IDFT = GenericDFT<Size, true>;


template<std::size_t Size, bool Inverse, typename Signal>
class DFTImpl;

template<std::size_t Size, bool Inverse, typename Signal>
DFTImpl<Size, Inverse, Signal> transform(GenericDFT<Size, Inverse> const& settings, Signal const& s) noexcept;

template<std::size_t Size, bool Inverse, typename Signal>
class DFTImpl
{
public:
    using sample_type = GenericIQ<typename Signal::value_type>;
    using value_type = typename Signal::value_type;

    using transform_type = GenericDFT<Size, Inverse>;
    using signal_type = Signal;

    [[gnu::pure]]
    constexpr sample_type operator()(Index m) const noexcept {
        constexpr value_type sign = Inverse ? 1 : -1;
        sample_type result{};

        for (Index n = 0; n < transform_type::size; ++n) {
            result += s(n) * exp(sign * generic_two_pi<value_type> * n * m / transform_type::size);
        }

        return Inverse ? (result / value_type(transform_type::size)) : result;
    }

private:
    friend DFTImpl transform<Size, Inverse, Signal>(transform_type const&, Signal const&) noexcept;

    constexpr DFTImpl(transform_type const&, signal_type const& s) noexcept : s(s) {}

    signal_type const s;
};

template<std::size_t Size, bool Inverse, typename Signal>
DFTImpl<Size, Inverse, Signal> transform(GenericDFT<Size, Inverse> const& settings, Signal const& s) noexcept {
    return { settings, s };
}

} /* namespace dsp */
