#include <stdio.h>
#include <stdatomic.h>
#include <pthread.h>
#include <time.h>
#include <stdlib.h>
#include <sys/sysinfo.h>
#include <unistd.h>

#define NUM_ITERATIONS (1 << 27)

#define CACHE_LINE_SIZE 64

#define MAX_THREADS (CACHE_LINE_SIZE / sizeof(atomic_int))

atomic_int counters[MAX_THREADS];

// Defalut Thread value( - Single Thread - )
int NUM_THREADS = 1;

// Final Value
atomic_int finalValue = ATOMIC_VAR_INIT(0);

// Initializer
void init() {

    for (int i = 0; i < NUM_THREADS; i++) {
        
        int value = 0;
        atomic_init(&counters[i],value);
    }
}

// Incrementing Function
void* increase(void* arg) {
    
    int threadId = *((int*)arg);

    int elementsPerThread = NUM_ITERATIONS / NUM_THREADS;

    // Increments
    for (int i = 0; i < elementsPerThread; i++) {
        
        atomic_fetch_add(&counters[threadId], 1);
    }
    
    // Final value addition
    atomic_fetch_add(&finalValue, atomic_load(&counters[threadId]));
    
    return NULL;
}

// Printing
int print() {

    printf("Threads: %d\n", NUM_THREADS);

    for (int i = 0; i < NUM_THREADS; i++) {

        printf("Counters[%d] - %p - %d\n", i, (void*)&counters[i], counters[i]);
    }

    atomic_int value = atomic_load(&finalValue);
    printf("Final Value - %d\n", value);
    
    return 0;
}

int main(int argc, char* argv[]) {

    // Validatin
    if (argc != 2) {
        printf("Usage: %s <num_threads>\n", argv[0]);
        return 1;
    }

    // Update Number of Threads
    NUM_THREADS = atoi(argv[1]);

    if (NUM_THREADS <= 0 || NUM_THREADS > MAX_THREADS) {

        printf("Invalid number of threads. Maximum supported: %ld\n", MAX_THREADS);
        
        return 1;
    }

    pthread_t threads[MAX_THREADS];

    init();

    int threadsIDs[MAX_THREADS];

    // Threads creations
    for (int i = 0; i < NUM_THREADS; i++) {
        
        // IDs Init
        threadsIDs[i] = i;

        // Threads Creation
        pthread_create(&threads[i], NULL, (void *(*)(void *)) &increase, &threadsIDs[i]);

    }

    for (int i = 0; i < NUM_THREADS; i++) {
        
        pthread_join(threads[i], NULL);
    }

    print();

    // system("{ /usr/bin/time -f 'Tiempo real: %e s, Tiempo de CPU: %U s, Memoria: %M KB' perf stat -d ./benchmarkOne 8 2>&1 | grep 'L1-dcache-loads\|L1-dcache-load-misses\|Tiempo';echo '$';} | sed -e 's/\x1B\[[0-9;]\+[A-Za-z]//g' -e '/^$/d' >> output.txt");
    // system("{ /usr/bin/time -f \"Tiempo real: %e s, Tiempo de CPU: %U s, Memoria: %M KB\" perf stat -d ./benchmarkOne 8 2>&1 | grep \"L1-dcache-loads\\|L1-dcache-load-misses\\|Tiempo\"; echo '$';} | sed -e 's/\\x1B\\[[0-9;]\\+[A-Za-z]//g' -e '/^$/d' >> output.txt");
    

    return 0;
}

