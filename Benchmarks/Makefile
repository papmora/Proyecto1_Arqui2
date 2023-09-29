CC = gcc
CXX = g++
CFLAGS = -Wall -pthread
CXXFLAGS = -Wall -pthread

.PHONY: all clean

all: bench1_c bench2_cpp bench_quicksort bench_slowsort bench_bogosort bench_directsharing bench_falsesharing benchmark

bench1_c: bench1.c
	$(CC) $(CFLAGS) -o $@ $^

bench_quicksort: bench_quicksort.c
	$(CC) $(CFLAGS) -o $@ $^

bench_slowsort: bench_slowsort.c
	$(CC) $(CFLAGS) -o $@ $^

bench_bogosort: bench_bogosort.c
	$(CC) $(CFLAGS) -o $@ $^

bench_directsharing: bench_directsharing.c
	$(CC) $(CFLAGS) -o $@ $^

bench_falsesharing: bench_falsesharing.c
	$(CC) $(CFLAGS) -o $@ $^

bench2_cpp: bench2.cpp
	$(CXX) $(CXXFLAGS) -o $@ $^

benchmark: benchmark.c
	$(CC) $(CFLAGS) -o $@ $^

clean:
	rm -f bench1_c bench2_cpp bench_quicksort bench_slowsort bench_directsharing bench_falsesharing benchmark bench1_c_* bench2_cpp_* bench_quicksort_* bench_slowsort_* bench_bogosort_* bench_directsharing_* bench_falsesharing_*