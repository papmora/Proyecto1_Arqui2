#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/time.h>
#include <sys/sysinfo.h>

#define TIMES 10

long numCores;

// Execution
void benchmark(const char *bench, long cores) {

    char perf_variations_command[100];

    for (int i = 1; i <= TIMES; i++) {

        sprintf(perf_variations_command, "perf stat -d ./%s %ld", bench, cores);
        system(perf_variations_command);
    }
    
}

int main(int argc, char *argv[]) {

    if (argc != 3) {

        printf("Usage: %s <benchmark> <cores>\n", argv[0]);
        printf("Benchmark options: BenchmarkOne or BenchmarkTwo\n");

        return 1;
    }

    // Selected Benchmark:
    const char *bench = argv[1];
    printf("This: %s\n", bench);

    // Number of Cores:
    long cores = atoi(argv[2]);

    // Conditional:
    if (strcmp(bench, "benchmarkOne") == 0) {
        printf("Entreee\n");
        benchmark("benchmarkOne", cores);

    } else if (strcmp(bench, "benchmarkTwo") == 0) {
        benchmark("benchmarkTwo", cores);

    } else {
        printf("Invalid option: %s\n", bench);
        return 1;
    }

    printf("Benchmarks completed\n");
    return 0;
}