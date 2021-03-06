#include "../dsp.hpp"

#include "filter.hpp"

#include <cstdlib>
#include <iostream>

using namespace dsp;

using input_sample_t = IQ;
using output_sample_t = float;

static constexpr std::size_t fs_in = 2400000;
static constexpr std::size_t fs_middle = 240000;
static constexpr std::size_t fs_out = 48000;

static constexpr std::size_t block_size = 51200;
static constexpr std::size_t block_count = 4;
static constexpr std::size_t output_block_size =
    block_size * fs_out / fs_in;

static std::array<input_sample_t, block_size * block_count> input{};

const auto pipeline = resample<std::ratio<fs_out, fs_middle>>(
    demod(
        WBFM(fs_middle),
        resample<std::ratio<fs_middle, fs_in>>(
            filter(
                wbfm_filter,
                periodic<block_size*block_count>(source(input))))));

int main() {
    std::array<output_sample_t, output_block_size> output;

    std::ptrdiff_t block = 0;

    while (std::cin.read(reinterpret_cast<char*>(&input[block * block_size]),
                         block_size * sizeof(input_sample_t))) {
        auto const start = block*output_block_size;
        for (Index n = 0; n < output_block_size; ++n)
            output[n] = pipeline(n + start);

        std::cout.write(reinterpret_cast<char const*>(output.data()),
                        output_block_size * sizeof(output_sample_t));

        block = (block + 1) % block_count;
    }

    auto const start = block*output_block_size;

    if (auto block_size = (std::cin.gcount() / sizeof(input_sample_t))) {
        auto output_block_size =
            block_size * fs_out / fs_in;

        for (Index n = 0; n < output_block_size; ++n)
            output[n] = pipeline(n + start);

        std::cout.write(reinterpret_cast<char const*>(output.data()),
                        output_block_size * sizeof(output_sample_t));
    }

    return 0;
}
