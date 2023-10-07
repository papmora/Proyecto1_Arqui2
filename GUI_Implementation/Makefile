CC = gcc
CXX = g++
CFLAGS = -Wall -lpthread
CXXFLAGS = -Wall -lpthread

.PHONY: all clean

all: benchmarkOne benchmarkTwo benchmarkOneS1 benchmarkOneS2 benchmarkTwoS1 benchmarkTwoS2 controller systemInfo

benchmarkOne: benchmarkOne.c
	$(CC) -o $@ $^ $(CFLAGS)

benchmarkTwo: benchmarkTwo.cpp
	$(CXX) -o $@ $^ $(CXXFLAGS) 

benchmarkOneS1: benchmarkOneS1.c
	$(CC)  -o $@ $^ $(CFLAGS)

benchmarkOneS2: benchmarkOneS2.c
	$(CC) -o $@ $^ $(CFLAGS) 

benchmarkTwoS1: benchmarkTwoS1.cpp
	$(CXX)  -o $@ $^ $(CXXFLAGS)

benchmarkTwoS2: benchmarkTwoS2.cpp
	$(CXX)  -o $@ $^ $(CXXFLAGS)

controller: controller.c
	$(CC)  -o $@ $^ $(CFLAGS)

systemInfo: systemInfo.c
	$(CC)  -o $@ $^ $(CFLAGS)

# delete: delete.c
# 	$(CC) $(CFLAGS) -o $@ $^

clean:
	rm -f controller benchmarkOne benchmarkTwo benchmarkOneS1 benchmarkOneS2 benchmarkTwoS1 benchmarkTwoS2 systemInfo perf.data perf.data.old