CC = gcc
CXX = g++
CFLAGS = -Wall -lpthread -O3
CXXFLAGS = -Wall -lpthread -O3

.PHONY: all clean

all: bench_falseSharing false_sharing_c

# bench1_c: bench1.c
# 	$(CC) $(CFLAGS) -o $@ $^

false_sharing_c: false_sharing.c
	$(CC) $(CFLAGS) -o $@ $^

# bench2_cpp: bench2.cpp
# 	$(CXX) $(CXXFLAGS) -o $@ $^

# benchmark: benchmark.c
# 	$(CC) $(CFLAGS) -o $@ $^

bench_falseSharing: bench_falseSharing.c
	$(CC) $(CFLAGS) -o $@ $^

clean:
	rm -f bench1_c false_sharing_c bench2_cpp benchmark bench_falseSharing bench1_c_* bench2_cpp_* bench_falseSharing_* perf.data