CXX = g++
CXXFLAGS = -g -O0 -std=c++11 -Wall -Wextra

all: kadane

run: kadane
	./kadane

kadane: kadane.cc
	$(CXX) -o $@ $(CXXFLAGS) $<

clean:
	rm -f kadane *~ *.tmp res

