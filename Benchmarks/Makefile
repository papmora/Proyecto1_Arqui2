CC = gcc
CXX = g++
CFLAGS = -Wall
CXXFLAGS = -Wall

.PHONY: all clean

all: bench1_c bench2_cpp

bench1_c: bench1.c
	$(CC) $(CFLAGS) -o $@ $^
	./$@

bench2_cpp: bench2.cpp
	$(CXX) $(CXXFLAGS) -o $@ $^
	./$@

clean:
	rm -f bench1_c bench2_cpp