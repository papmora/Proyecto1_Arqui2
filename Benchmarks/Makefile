CC = gcc
CXX = g++
CFLAGS = -Wall -pthread
CXXFLAGS = -Wall -pthread

.PHONY: all clean

all: bench1_c bench2_cpp bench_quicksort benchmark

bench1_c: bench1.c
	$(CC) $(CFLAGS) -o $@ $^

bench_quicksort: bench_quicksort.c
	$(CC) $(CFLAGS) -o $@ $^

bench2_cpp: bench2.cpp
	$(CXX) $(CXXFLAGS) -o $@ $^

benchmark: benchmark.c
	$(CC) $(CFLAGS) -o $@ $^

clean:
	rm -f bench1_c bench2_cpp bench_quicksort benchmark bench1_c_* bench2_cpp_* bench_quicksort_*