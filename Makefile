CC = gcc
CXX = g++
CFLAGS = -Wall -lpthread
CXXFLAGS = -Wall -lpthread -O3

.PHONY: all clean

all: benchmarkOne controller systemInfo

benchmarkOne: benchmarkOne.c
	$(CC) $(CFLAGS) -o $@ $^

controller: controller.c
	$(CC) $(CFLAGS) -o $@ $^

systemInfo: systemInfo.c
	$(CC) $(CFLAGS) -o $@ $^

clean:
	rm -f controller benchmarkOne systemInfo perf.data perf.data.old