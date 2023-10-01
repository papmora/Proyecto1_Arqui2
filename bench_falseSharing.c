#include <stdio.h>
#include <stdatomic.h>
#include <pthread.h>
#include <time.h>
#include <stdlib.h>

#define NUM_ITERATIONS (1 << 27)

#define NUM_THREADS 1

atomic_int counters[NUM_THREADS];

// Final Value
atomic_int finalValue = ATOMIC_VAR_INIT(0);

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

void print() {

    for (int i = 0; i < NUM_THREADS; i++) {

        printf("Counters[%d] - %p - %d\n", i, (void*)&counters[i], counters[i]);
    }

    atomic_int value = atomic_load(&finalValue);
    printf("Final Value - %d\n", value);
}

int main() {
    
    pthread_t threads[NUM_THREADS];

    init();

    int threadsIDs[NUM_THREADS];

    // Threads creations
    for (int i = 0; i < NUM_THREADS; i++) {
        
        // IDs Init
        threadsIDs[i] = i;

        // Threads Creation
        pthread_create(&threads[i], NULL, increase, &threadsIDs[i]);

    }

    for (int i = 0; i < NUM_THREADS; i++) {
        
        pthread_join(threads[i], NULL);
    }

    print();

    return 0;
}

