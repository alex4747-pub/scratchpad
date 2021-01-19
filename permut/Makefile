CXX = g++
CXXFLAGS = -g -O0 -std=c++11 -Wall -Wextra

all: run

run: permut
	./permut

permut: permut.cc
	$(CXX) -o $@ $(CXXFLAGS) $<

clean:
	rm -f permut *~ *.tmp res*

