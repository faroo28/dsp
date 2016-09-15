CXX=g++
CXXFLAGS += -std=c++14
LIBS = -lm

ifdef DEBUG
	CXXFLAGS += -g -O0
else
	CXXFLAGS += -flto -Ofast -mavx2 -DNDEBUG
endif

default: all

all: perf demod mod

perf: perf.cpp filter.hpp
	$(CXX) $(CXXFLAGS) -o perf perf.cpp $(LIBS)

demod: demod.cpp filter.hpp
	$(CXX) $(CXXFLAGS) -o demod demod.cpp $(LIBS)

mod: mod.cpp filter.hpp
	$(CXX) $(CXXFLAGS) -o mod mod.cpp $(LIBS)

clean:
	rm -f perf demod mod
