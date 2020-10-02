.PHONY: all clean generate run-test format tidy cpplint

FORMAT  = clang-format
TIDY    = clang-tidy
CPPLINT = cpplint

CXX = g++
CXXFLAGS = -g -O0 -std=c++11 -Wall -Wextra -I.

SRCS = \
	mem_debug.cc \
	mem_debug_main.cc

INCS = \
	mem_debug.h

OBJS=$(patsubst %.cc, %.o, $(SRCS))

all: mem_debug

mem_debug: $(OBJS)
	$(CXX) -o $@ $(CXXFLAGS) $^

%.o: %.cc $(INCS)
	$(CXX) -c $(CXXFLAGS) $<


clean:
	rm -rf mem_debug *.o *~

run-test: mem_debug
	./mem_debug

format: $(SRCS) $(INCS) Makefile
	$(FORMAT) --style=google -i $(SRCS) $(INCS)

tidy: $(SRCS) $(INCS) Makefile
	$(TIDY) --fix -extra-arg-before=-xc++ $(SRCS) $(INCS) -- $(CXXFLAGS)

cpplint: $(SRCS) $(INCS) Makefile
	$(CPPLINT) $(SRCS) $(INCS)

