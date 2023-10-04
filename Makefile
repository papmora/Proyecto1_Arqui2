CC = gcc
CXX = g++
CFLAGS = -Wall -lpthread
CXXFLAGS = -Wall -lpthread

.PHONY: all clean

all: benchmarkOne benchmarkTwo benchmarkOneS1 benchmarkOneS2 controller systemInfo

benchmarkOne: benchmarkOne.c
	$(CC) $(CFLAGS) -o $@ $^

benchmarkTwo: benchmarkTwo.cpp
	$(CXX) $(CXXFLAGS) -o $@ $^

benchmarkOneS1: benchmarkOneS1.c
	$(CC) $(CFLAGS) -o $@ $^

benchmarkOneS2: benchmarkOneS2.c
	$(CC) $(CFLAGS) -o $@ $^

controller: controller.c
	$(CC) $(CFLAGS) -o $@ $^

systemInfo: systemInfo.c
	$(CC) $(CFLAGS) -o $@ $^

# delete: delete.c
# 	$(CC) $(CFLAGS) -o $@ $^

clean:
	rm -f controller benchmarkOne benchmarkTwo benchmarkOneS1 benchmarkOneS2 systemInfo perf.data perf.data.old