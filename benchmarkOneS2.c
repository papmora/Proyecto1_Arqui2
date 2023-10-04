#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>

#define NUM_ITERATIONS (1 << 27)

// Adjust the number of threads based on your requirements
#define MAX_THREADS 16

// Defalut Thread value( - Single Thread - )
int NUM_THREADS = 1;

// Final Value
volatile int finalValue = 0;

// Struct to hold thread-specific data
struct ThreadData {
    int counter;
};

// Incrementing Function
void* increase(void* arg) {
    struct ThreadData* data = (struct ThreadData*)arg;

    int elementsPerThread = NUM_ITERATIONS / MAX_THREADS;

    for (int i = 0; i < elementsPerThread; i++) {
        data->counter++;
    }

    // Final value addition
    finalValue += data->counter;

    return NULL;
}

// Printing
int print() {
    printf("Threads: %d\n", MAX_THREADS);

    printf("Final Value - %d\n", finalValue);
    return 0;
}

int main() {
    // Threads creations
    pthread_t threads[MAX_THREADS];
    struct ThreadData threadData[MAX_THREADS];

    for (int i = 0; i < MAX_THREADS; i++) {
        threadData[i].counter = 0;

        // Threads Creation
        pthread_create(&threads[i], NULL, increase, &threadData[i]);
    }

    for (int i = 0; i < MAX_THREADS; i++) {
        pthread_join(threads[i], NULL);
    }

    print();

    return 0;
}

