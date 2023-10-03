CC = gcc
CXX = g++
CFLAGS = -Wall -lpthread
CXXFLAGS = -Wall -lpthread -O3

.PHONY: all clean

all: benchmarkOne app test

benchmarkOne: benchmarkOne.c
	$(CC) $(CFLAGS) -o $@ $^

app: app.c
	$(CC) $(CFLAGS) -o $@ $^

clean:
	rm -f app benchmarkOne perf.data perf.data.old