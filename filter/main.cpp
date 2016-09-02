#include "filter.hpp"

#include "../plot.hpp"

#include <boost/lexical_cast.hpp>

#include <cmath>
#include <iomanip>
#include <iostream>
#include <string>

int main(int argc, char const* const argv[]) {
    if (argc < 4) {
        std::cerr << "Usage: "
                  << argv[0]
                  << " filter_type filter_size sampling_frequency filter_params..."
                  << std::endl;
        return 1;
    }

    std::string type = argv[1];

    std::size_t size, fs;
    float cutoff_or_center, bandwidth;

    try {
        size = boost::lexical_cast<std::size_t>(argv[2]);
        fs = boost::lexical_cast<std::size_t>(argv[3]);

        if (type == "lowpass" || type == "highpass") {
            if (argc < 5) {
                std::cerr << "Usage: "
                          << argv[0] << ' ' << argv[1]
                          << " filter_size sampling_frequency cutoff"
                          << std::endl;
                return 1;
            }
        } else if (type == "bandpass" || type == "bandstop") {
            if (argc < 6) {
                std::cerr << "Usage: "
                          << argv[0] << ' ' << argv[1]
                          << " filter_size sampling_frequency center bandwidth"
                          << std::endl;
                return 1;
            }

            bandwidth = boost::lexical_cast<float>(argv[5]);
        } else {
            std::cerr << "error: invalid filter type '" << argv[1] << '\'' << std::endl;
            return 1;
        }

        cutoff_or_center = std::abs(boost::lexical_cast<float>(argv[4]));
    } catch (boost::bad_lexical_cast const&) {
        std::cerr << "error: invalid argument value" << std::endl;
        return 1;
    }

    std::vector<float> taps;

    if (type == "lowpass") {
        taps = lowpass(size, fs, cutoff_or_center);
    } else if (type == "highpass") {
        taps = highpass(size, fs, cutoff_or_center);
    } else if (type == "bandpass") {
        taps = bandpass(size, fs, cutoff_or_center, bandwidth);
    } else if (type == "bandstop") {
        taps = bandstop(size, fs, cutoff_or_center, bandwidth);
    }

    auto resp = response(taps);

    std::cerr << "Filter taps:\n"
              << dsp::plot(taps.begin(), taps.end()) << '\n'
              << "Frequency response:\n"
              << dsp::plot(resp.begin(), resp.end()) << '\n'
              << "Phase response (⋅π):\n"
              << dsp::plot<dsp::detail::ScaleFilter<1, 3, dsp::detail::PhiFilter>>(resp.begin(), resp.end())
              << std::endl;

    std::cout << std::setprecision(10) << std::defaultfloat;

    std::cout << "constexpr FIRFilter<" << size << "> taps = {{{" << std::flush;

    if (size) {
        std::cout << "\n    " << taps[0] << std::flush;

        for (std::size_t n = 1; n < size; ++n)
            std::cout << ",\n    " << taps[n] << std::flush;
    }

    std::cout << "\n}}};" << std::endl;

    return 0;
}
