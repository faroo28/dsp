#pragma once

#include <chrono>
#include <functional>
#include <iostream>

namespace dsp
{

namespace detail
{
    struct defer
    {
        explicit defer(std::function<void()>&& fn) : fn(std::move(fn)) {}

        ~defer() {
            fn();
        }

        std::function<void()> fn;
    };
} /* namespace detail */

template<typename M, typename F>
auto perf(M&& msg, F&& fn) {
    auto start = std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::high_resolution_clock::now().time_since_epoch()).count();
    detail::defer _([start,&msg]{
        std::cout << std::forward<M>(msg) << ' ' << (std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::high_resolution_clock::now().time_since_epoch()).count() - start) << "µs" << std::endl;
    });

    return std::forward<F>(fn)();
}

} /* namespace dsp */
