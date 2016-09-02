#include <vector>

#include "../iq.hpp"

std::vector<float> filter(std::vector<float> const& response);

std::vector<float> lowpass(std::size_t size, float fs, float cutoff);

std::vector<float> highpass(std::size_t size, float fs, float cutoff);

std::vector<float> bandpass(std::size_t size, float fs, float center, float bandwidth);

std::vector<float> bandstop(std::size_t size, float fs, float center, float bandwidth);

std::vector<dsp::IQ> response(std::vector<float> const& filter);
