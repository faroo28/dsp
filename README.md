# dsp
This is an **experimental**, very rudimental yet quite interesting and at times even useful C++ DSP library.

**WARNING: I am not a DSP expert, just a hobbyist. I do not claim nor guarantee in any way code or algorithm correctness.**

That said, I'm really happy with this library. I'm using it as a toolbox to experiment with DSP pipelines and algorithms and it's been quite useful. Of course it's far from complete: it lacks mixer functions to combine signals,
a FFT implementation and many other things.

I wrote all code by myself, mostly using Rick G. Lyons's *Understanding Digital Signal Processing* as reference. After nearly one year I still understand most code without problems; this should be a good sign. :grin:

My approach was quite different from that of other libraries. The library was born as an experiment with compiler optimizations and above all vectorization, thus pipelines are entirely configured at compile time and immutable at runtime. The code is very generic, templates and constexpr are everywhere. Pipelines are built by functional-style composition; evaluation is lazy, but the library tries to compute in advance everything possible at compile time.

```c++
#include "dsp.hpp"

#include <array>
#include <tuple>

constexpr dsp::FIRFilter<64> fir = {{{ /* taps... */ }}};
std::array<float, 512> input = {{ /* samples... */ }};

auto pipeline = dsp::transform(dsp::DFT<512>(),
    dsp::filter(fir,
        dsp::zero_pad<std::tuple_size<decltype(input)>::value>(
            dsp::source(input))));

// With 0 <= n < 512:
// auto nth_sample = pipeline(n);
```

The experiment was not successful in itself: code is inlined properly, but never vectorized. Until now, I've not been able to fix it. Performance is not good; however, the library works and is quite usable.


## Examples

Each folder contains makefiles for gcc. The library uses GNU function attributes, so it won't build as is on VC++. Full C++14 support is required.

The folder [filter](filter) contains a very simple command-line FIR filter design tool. It outputs C++ variable definitions intended for use with this library. It also outputs some graphs on `stderr` with frequency and phase response.

The folder [wbfm](wbfm) contains a working wideband FM demodulator. It reads complex samples from `stdin` in the form of 32-bit float pairs and outputs real 32-bit float samples on `stdout`. The input must be sampled at 2.4 MHz and already tuned with the carrier base frequency centered on zero. The output is downsampled to 48 kHz.

```sh
$ ffmpeg -i input_file -f f32le -c:a pcm_f32le - 2> /dev/null |
    ./demod | ffplay -f f32le -ar 48k -ac 1 - 2> /dev/null
```
