#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>
#include <stdint.h>

#define NUM_ITERATIONS (1 << 27)

#define CACHE_LINE_SIZE 64

// Adjust the number of threads based on your requirements
#define MAX_THREADS (CACHE_LINE_SIZE / sizeof(int))

// Defalut Thread value( - Single Thread - )
int NUM_THREADS = 1;

// Final Value
volatile int finalValue = 0;

// Thread-local variables to hold counters
__thread int threadCounter = 0;

// Incrementing Function
void* increase(void* arg) {
    
    int elementsPerThread = NUM_ITERATIONS / NUM_THREADS;

    printf("Thread %ld: threadCounter address: %p\n", (long)arg, &threadCounter);

    for (int i = 0; i < elementsPerThread; i++) {
        threadCounter++;
    }

    // Final value addition
    finalValue += threadCounter;

    return NULL;
}

// Printing
int print() {

    printf("Threads: %d\n", NUM_THREADS);

    printf("Final Value - %d\n", finalValue);
    return 0;
}

int main(int argc, char* argv[]) {

    // Validation
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

    // Threads creations
    pthread_t threads[MAX_THREADS];

    for (int i = 0; i < NUM_THREADS; i++) {

        intptr_t thread_id = (intptr_t)i;

        // Threads Creation
        pthread_create(&threads[i], NULL, increase, (void*)thread_id);
    }

    for (int i = 0; i < NUM_THREADS; i++) {

        pthread_join(threads[i], NULL);
    }

    print();

    return 0;
}
