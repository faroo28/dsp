#include "../dsp.hpp"

#include "filter.hpp"

#include <cstdlib>
#include <tuple>

using namespace dsp;

std::array<IQ, 51200> input{};

const auto pipeline = resample<std::ratio<48000, 240000>>(
    demod(
        WBFMDemod(240000),
        resample<std::ratio<240000, 2400000>>(
            filter(
                wbfm_filter,
                zero_pad<std::tuple_size<decltype(input)>::value>(source(input))))));

int main() {
    std::array<volatile float, 8192> output{};

    auto run = [&output](std::size_t count){
        for (Index n = 0; n < count; ++n)
            output[n] = pipeline(n);
    };

    perf("demod wbfm (50->1):", [&run]{ run(1); });

    perf("demod wbfm (200->4):", [&run]{ run(4); });

    perf("demod wbfm (400->8):", [&run]{ run(8); });

    perf("demod wbfm (6400->128):", [&run]{ run(128); });

    perf("demod wbfm (192000->384):", [&run]{ run(384); });

    perf("demod wbfm (51200->1024):", [&run]{ run(1024); });
}
