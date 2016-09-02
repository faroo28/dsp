default: all

all: filter

filter: main.cpp filter.cpp filter.hpp dyn_blackman.hpp dyn_dft.hpp dyn_signal.hpp
	g++ -std=c++14 -flto -Ofast -o filter main.cpp filter.cpp -lm

clean:
	rm -f filter
