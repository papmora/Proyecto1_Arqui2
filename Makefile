CC = gcc
CXX = g++
CFLAGS = -Wall
CXXFLAGS = -Wall

.PHONY: all clean

all: bench1_c bench2_cpp benchmark bench_falseSharing

bench1_c: bench1.c
	$(CC) $(CFLAGS) -o $@ $^ -lpthread

bench2_cpp: bench2.cpp
	$(CXX) $(CXXFLAGS) -o $@ $^ 

benchmark: benchmark.c
	$(CC) $(CFLAGS) -o $@ $^

bench_falseSharing: bench_falseSharing.c
	$(CC) $(CFLAGS) -o $@ $^ -lpthread

clean:
	rm -f bench1_c bench2_cpp benchmark bench_falseSharing bench1_c_* bench2_cpp_* bench_falseSharing_*