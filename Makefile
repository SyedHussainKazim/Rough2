CXX = g++
CXXFLAGS = -std=c++17

all: debug optimized

debug: CXXFLAGS += -g
debug: ltcg_debug

optimized: CXXFLAGS += -O2
optimized: ltcg_optimized

ltcg_debug: main.cpp
	$(CXX) $(CXXFLAGS) -o ltcg_debug main.cpp

ltcg_optimized: main.cpp
	$(CXX) $(CXXFLAGS) -o ltcg_optimized main.cpp

clean:
	rm -f ltcg_debug ltcg_optimized
