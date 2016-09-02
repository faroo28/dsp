#include "filter.hpp"

#include "../dsp.hpp"

#include "dyn_blackman.hpp"
#include "dyn_dft.hpp"
#include "dyn_signal.hpp"

using namespace dsp;

std::vector<float> filter(std::vector<float> const& response)
{
    auto const size = response.size();

    std::vector<float> result(size);

    auto const shift = size/2 - 1 + (size % 2);

    auto const idft = transform(DynIDFT(size), periodic(size, source(response)));
    auto const wnd = window(DynBlackmanWindow(size));

    for (Index n = 0; n < size; ++n)
        result[n] = I(idft(n - shift)) * wnd(n);

    return result;
}

std::vector<float> lowpass(std::size_t size, float fs, float cutoff)
{
    auto const real_size = size - 1 + (size % 2);

    std::vector<float> response(real_size);

    auto const fstep = float(fs) / real_size;
    std::size_t const cutoff_bin_low = std::floor(cutoff / fstep);
    std::size_t const cutoff_bin_high = real_size - cutoff_bin_low;

    for (Index n = 0; n < real_size; ++n) {
        response[n] = (n <= cutoff_bin_low || n >= cutoff_bin_high) ? 1.0f : 0.0f;
    }

    auto taps = filter(response);
    taps.resize(size);

    return taps;
}

std::vector<float> highpass(std::size_t size, float fs, float cutoff)
{
    auto taps = lowpass(size, fs, fs/2 - cutoff);

    for (Index n = 0; n < size; n += 2)
        taps[n] = -taps[n];

    return taps;
}

std::vector<float> bandpass(std::size_t size, float fs, float center, float bandwidth)
{
    auto taps = lowpass(size, fs, bandwidth/2);

    auto const astep = two_pi * bandwidth / fs;

    for (Index n = 0; n < size; ++n)
        taps[n] *= -std::sin(astep * n);

    return taps;
}

std::vector<float> bandstop(std::size_t size, float fs, float center, float bandwidth)
{
    auto taps = highpass(size, fs, bandwidth/2);

    auto const astep = two_pi * bandwidth / fs;

    for (Index n = 0; n < size; ++n)
        taps[n] *= -std::sin(astep * n);

    return taps;
}

std::vector<IQ> response(std::vector<float> const& filter) {
    auto const size = filter.size();

    std::vector<IQ> response(size);

    auto const dft = transform(DynDFT(size), source(filter));

    for (Index n = 0; n < size; ++n)
        response[n] = dft(n);

    return response;
}
