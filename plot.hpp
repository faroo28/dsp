#pragma once

#include "sample.hpp"

#include <cmath>
#include <algorithm>
#include <iomanip>
#include <ostream>

namespace dsp
{

namespace detail
{
    struct MagFilter;

    template<typename Iterator, typename Filter>
    class Plot;
}

template<typename Filter = detail::MagFilter, typename Iterator>
constexpr detail::Plot<Iterator, Filter> plot(Iterator const& first, Iterator const& last) noexcept;

namespace detail
{
    template<typename Iterator, typename Filter>
    class Plot
    {
    public:
        template<typename CharT, typename Traits>
        std::basic_ostream<CharT, Traits>& operator()(std::basic_ostream<CharT, Traits>& stream) const;

    private:
        friend Plot dsp::plot<Filter, Iterator>(Iterator const&, Iterator const&) noexcept;

        constexpr Plot(Iterator const& first, Iterator const& last, Filter const& flt = Filter{}) noexcept
            : first(first), last(last), flt(flt)
            {}

        static constexpr std::size_t number_width(std::int64_t number) noexcept;

        template<typename T>
        static constexpr char const* point(std::int64_t line, T const& p, bool first = false) noexcept {
            return (std::floor(p*10) == line) ? "▪" : (first ? (line ? "|" : "+") : (line ? " " : "-"));
        }

        Iterator const first;
        Iterator const last;
        Filter const flt;
    };

    template<typename Iterator, typename Filter>
    template<typename CharT, typename Traits>
    std::basic_ostream<CharT, Traits>&
    Plot<Iterator, Filter>::operator()(std::basic_ostream<CharT, Traits>& stream) const {
        std::int64_t top =
            std::floor(flt(*std::max_element(first, last, [this](auto const& s1, auto const& s2) { return flt(s1) < flt(s2); })) * 10);
        std::int64_t bottom =
            std::floor(flt(*std::min_element(first, last, [this](auto const& s1, auto const& s2) { return flt(s1) < flt(s2); })) * 10);

        auto width = std::max(std::max(number_width(top), number_width(bottom)), 2ul);

        for (auto line = top; line >= bottom; --line) {
            stream << ' ' << std::right << std::setw(width);

            if (std::abs(line) < 10) {
                stream << ((line < 0) ? "-0" : "0");
            } else {
                stream << (line/10);
            }

            stream << '.' << (std::abs(line) % 10) << ' ';

            if (first != last) {
                auto it = first;

                stream << point(line, flt(*it), true);

                for (++it; it != last; ++it)
                    stream << point(line, flt(*it));
            }

            stream << '\n';
        }

        return stream;
    }

    template<typename Iterator, typename Filter>
    constexpr std::size_t Plot<Iterator, Filter>::number_width(std::int64_t number) noexcept {
        std::size_t result = 0;

        while (number) {
            number /= 10;
            ++result;
        }

        return result;
    }

    template<typename CharT, typename Traits, typename Iterator, typename Filter>
    inline std::basic_ostream<CharT, Traits>& operator<<(std::basic_ostream<CharT, Traits>& stream, Plot<Iterator, Filter> const& plt) {
        return plt(stream);
    }

    struct IFilter
    {
        template<typename T>
        [[gnu::const]]
        constexpr value_type_t<T> operator()(T const& s) const noexcept {
            return I(s);
        }
    };

    struct QFilter
    {
        template<typename T>
        [[gnu::const]]
        constexpr value_type_t<T> operator()(T const& s) const noexcept {
            return Q(s);
        }
    };

    struct MagFilter
    {
        template<typename T>
        [[gnu::const]]
        constexpr value_type_t<T> operator()(T const& s) const noexcept {
            return mag(s);
        }
    };

    struct PhiFilter
    {
        template<typename T>
        [[gnu::const]]
        constexpr value_type_t<T> operator()(T const& s) const noexcept {
            return phi(s);
        }
    };

    template<std::int64_t Num, std::int64_t Den = 1, typename Prev = void>
    class ScaleFilter
    {
    public:
        constexpr ScaleFilter(Prev const& prev = Prev{}) noexcept
            : prev(prev)
            {}

        template<typename T>
        [[gnu::const]]
        constexpr std::result_of_t<Prev(T const&)> operator()(T const& s) const noexcept {
            return prev(s) * value_type_t<T>(Num) / value_type_t<T>(Den);
        }

    private:
        Prev const prev;
    };

    template<std::int64_t Num, std::int64_t Den>
    struct ScaleFilter<Num, Den, void>
    {
        template<typename T>
        [[gnu::const]]
        constexpr T operator()(T const& s) const noexcept {
            return s * value_type_t<T>(Num) / value_type_t<T>(Den);
        }
    };
} /* namespace detail */

template<typename Filter, typename Iterator>
inline constexpr detail::Plot<Iterator, Filter> plot(Iterator const& first, Iterator const& last) noexcept {
    return { first, last };
}


template<typename Iterator>
inline constexpr detail::Plot<Iterator, detail::PhiFilter> plot_phase(Iterator const& first, Iterator const& last) noexcept {
    return plot<detail::PhiFilter>(first, last);
}

template<typename Iterator>
inline constexpr detail::Plot<Iterator, detail::IFilter> plot_real(Iterator const& first, Iterator const& last) noexcept {
    return plot<detail::IFilter>(first, last);
}

template<typename Iterator>
inline constexpr detail::Plot<Iterator, detail::QFilter> plot_imag(Iterator const& first, Iterator const& last) noexcept {
    return plot<detail::QFilter>(first, last);
}

} /* namespace dsp */
