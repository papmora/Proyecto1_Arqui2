CC = gcc
CXX = g++
CFLAGS = -Wall -lpthread
CXXFLAGS = -Wall -lpthread -O3

.PHONY: all clean

all: benchmarkOne benchmarkTwo app 

benchmarkOne: benchmarkOne.c
	$(CC) $(CFLAGS) -o $@ $^

benchmarkTwo: benchmarkTwo.cpp
	$(CXX) $(CXXFLAGS) -o $@ $^

app: app.c
	$(CC) $(CFLAGS) -o $@ $^

clean:
	rm -f app benchmarkOne perf.data perf.data.old