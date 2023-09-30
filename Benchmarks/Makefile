CC = gcc
CXX = g++
CFLAGS = -Wall -pthread
CXXFLAGS = -Wall -pthread

.PHONY: all clean

all: bench_bubblesort bench_directsharing bench_falsesharing benchmark

bench_bubblesort: bench_bubblesort.c
	$(CC) $(CFLAGS) -o $@ $^

bench_directsharing: bench_directsharing.c
	$(CC) $(CFLAGS) -o $@ $^

bench_falsesharing: bench_falsesharing.c
	$(CC) $(CFLAGS) -o $@ $^

benchmark: benchmark.c
	$(CC) $(CFLAGS) -o $@ $^

clean:
	rm -f bench_bubblesort bench_directsharing bench_falsesharing benchmark bench_bubblesort_* bench_directsharing_* bench_falsesharing_* 