#pragma once

#include "sample.hpp"

#include <iterator>
#include <type_traits>

namespace dsp
{

namespace detail
{
    template<typename T>
    struct is_sequence
    {
        template<typename S>
        static constexpr bool is_seq(decltype(std::begin(std::declval<S const>()) + std::declval<std::size_t>()) const*) noexcept {
            return true;
        }

        template<typename S>
        static constexpr bool is_seq(...) {
            return false;
        }

        static constexpr bool value = is_seq<T>(0);
    };
} /* namespace detail */


template<typename T>
class ConstantSource;

template<typename T>
constexpr std::enable_if_t<!detail::is_sequence<T>::value, ConstantSource<T>> source(T const& value) noexcept;

template<typename T>
class ConstantSource
{
public:
    using sample_type = T;
    using value_type = value_type_t<sample_type>;

    [[gnu::pure]]
    constexpr sample_type operator()(Index n) const noexcept {
        return value;
    }

private:
    friend ConstantSource source<T>(T const&) noexcept;

    constexpr ConstantSource(sample_type const& value) noexcept
        : value(value)
        {}

    sample_type const value;
};

template<typename T>
constexpr std::enable_if_t<!detail::is_sequence<T>::value, ConstantSource<T>> source(T const& value) noexcept {
    return { value };
}


template<typename Sequence>
class SequenceSource;

template<typename Sequence>
constexpr std::enable_if_t<detail::is_sequence<Sequence>::value, SequenceSource<Sequence>> source(Sequence const& seq) noexcept;

template<typename Sequence>
class SequenceSource
{
public:
    using sample_type = typename Sequence::value_type;
    using value_type = value_type_t<sample_type>;

    using sequence_type = Sequence;

    [[gnu::pure]]
    constexpr sample_type operator()(Index n) const noexcept {
        return *(std::begin(seq) + n);
    }

private:
    friend SequenceSource source<Sequence>(Sequence const&) noexcept;

    constexpr SequenceSource(sequence_type const& seq) noexcept
        : seq(seq)
        {}

    sequence_type const& seq;
};

template<typename Sequence>
constexpr std::enable_if_t<detail::is_sequence<Sequence>::value, SequenceSource<Sequence>> source(Sequence const& seq) noexcept {
    return { seq };
}

} /* namespace dsp */
