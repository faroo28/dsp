default: all

all: perf demod

perf: perf.cpp filter.hpp
	g++ -std=c++14 -flto -Ofast -mavx2 -o perf perf.cpp -lm

demod: demod.cpp
	g++ -std=c++14 -flto -Ofast -mavx2 -o demod demod.cpp -lm

clean:
	rm -f perf demod
