cmake_minimum_required(VERSION 3.17)
project(Sorting_Algorithm_C C)

find_package(Threads REQUIRED)

set(CMAKE_C_STANDARD 99)

add_executable(Sorting_Algorithm_C main.c)
target_link_libraries(Sorting_Algorithm_C Threads::Threads)