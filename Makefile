CC = gcc
CXX = g++
CFLAGS = -Wall -lpthread
CXXFLAGS = -Wall -lpthread

.PHONY: all clean

all: benchmarkOne benchmarkTwo controller systemInfo delete

benchmarkOne: benchmarkOne.c
	$(CC) $(CFLAGS) -o $@ $^

benchmarkTwo: benchmarkTwo.cpp
	$(CXX) $(CXXFLAGS) -o $@ $^

controller: controller.c
	$(CC) $(CFLAGS) -o $@ $^

systemInfo: systemInfo.c
	$(CC) $(CFLAGS) -o $@ $^

delete: delete.c
	$(CC) $(CFLAGS) -o $@ $^

clean:
	rm -f controller benchmarkOne benchmarkTwo systemInfo delete perf.data perf.data.old