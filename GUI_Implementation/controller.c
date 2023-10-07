#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/time.h>
#include <sys/sysinfo.h>

#define TIMES 10

long numCores;

// Execution
void benchmark(const char *bench,long cores, const char *output ) {

    // char perf_variations_command[100];
    char perf_test[200];

    printf("Executing: %s\n\n",bench);
    
    for (int i = 1; i <= TIMES; i++) {

        // sprintf(perf_variations_command, "perf stat -d ./%s %ld", bench, cores);
        // system(perf_variations_command);

        sprintf(perf_test, "{ perf stat -d ./%s %ld 2>&1 | grep \"L1-dcache-loads\\|L1-dcache-load-misses\\|seconds time elapsed\"; echo '$';} | sed -e 's/\\x1B\\[[0-9;]\\+[A-Za-z]//g' -e '/^$/d' >> output_%s.txt", bench, cores,output);

        system(perf_test);
    }   
    
}

int main(int argc, char *argv[]) {

    if (argc != 4) {

        printf("Usage: %s <benchmark> <cores>\n", argv[0]);
        printf("Benchmark options: benchmarkOne, benchmarkTwo, benchmarkOneS1, benchmarkTwoS1,benchmarkOneS2, benchmarkTwoS2 \n");

        return 1;
    }

    // Selected Benchmark:
    const char *bench = argv[1];

    // Number of Cores:
    long cores = atoi(argv[2]);

    const char *output=argv[3];

    // Conditional:
    if (strcmp(bench, "benchmarkOne") == 0) {
        
        benchmark("benchmarkOne", cores,output);

    } else if (strcmp(bench, "benchmarkTwo") == 0) {
        benchmark("benchmarkTwo", cores,output);

    } else if (strcmp(bench, "benchmarkOneS1") == 0) {
        
        benchmark("benchmarkOneS1", cores,output);

    } else if (strcmp(bench, "benchmarkTwoS1") == 0) {
        benchmark("benchmarkTwoS1", cores,output);

    } else if (strcmp(bench, "benchmarkOneS2") == 0) {
        
        benchmark("benchmarkOneS2", cores,output);

    } else if (strcmp(bench, "benchmarkTwoS2") == 0) {
        benchmark("benchmarkTwoS2", cores,output);

    } else {
        printf("Invalid option: %s\n", bench);
        return 1;
    }

    printf("Benchmarks completed\n");
    return 0;
}