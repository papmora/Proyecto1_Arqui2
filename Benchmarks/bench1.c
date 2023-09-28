#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <stdint.h>
#include <inttypes.h>
#include <sys/time.h>
#include <sys/sysinfo.h>
#include <unistd.h>

#define ARRAY_SIZE 16
#define NUM_THREADS 4

// Alineación de 64 bytes para provocar false sharing
#define CACHE_LINE_SIZE 64

typedef struct {
    int data;
    char padding[CACHE_LINE_SIZE - sizeof(int)];
} CacheLine;

CacheLine sharedArray[ARRAY_SIZE];
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;

// Función que realiza operaciones de escritura en el arreglo compartido
void *writeToSharedArray(void *threadId) {
    intptr_t id = (intptr_t)threadId;  // Usar intptr_t para evitar advertencias
    int startIndex = id * (ARRAY_SIZE / NUM_THREADS);
    int endIndex = (id + 1) * (ARRAY_SIZE / NUM_THREADS);

    for (int i = startIndex; i < endIndex; i++) {
        // Provocar false sharing: Acceder y modificar elementos cercanos en la memoria.
        pthread_mutex_lock(&mutex);
        sharedArray[i].data = i;
        pthread_mutex_unlock(&mutex);
    }

    pthread_exit(NULL);
}

int main() {

    pthread_t threads[NUM_THREADS];
    struct timeval start, end;
    uint64_t elapsed;

    // Inicializar el arreglo compartido con valores iniciales
    for (int i = 0; i < ARRAY_SIZE; i++) {
        sharedArray[i].data = 0;
    }

    // Medir el tiempo de ejecución de la versión single-thread
    gettimeofday(&start, NULL);

    for (intptr_t i = 0; i < NUM_THREADS; i++) {
        pthread_create(&threads[i], NULL, writeToSharedArray, (void *)i);
    }

    for (int i = 0; i < NUM_THREADS; i++) {
        pthread_join(threads[i], NULL);
    }

    gettimeofday(&end, NULL);
    elapsed = (end.tv_sec - start.tv_sec) * 1000000 + (end.tv_usec - start.tv_usec);

    printf("Single-Thread Execution Time: %" PRIu64 " microseconds\n", elapsed);

    // Reinicializar el arreglo compartido
    for (int i = 0; i < ARRAY_SIZE; i++) {
        sharedArray[i].data = 0;
    }

    // Medir el tiempo de ejecución de la versión multi-thread
    gettimeofday(&start, NULL);

    for (intptr_t i = 0; i < NUM_THREADS; i++) {
        pthread_create(&threads[i], NULL, writeToSharedArray, (void *)i);
    }

    for (int i = 0; i < NUM_THREADS; i++) {
        pthread_join(threads[i], NULL);
    }

    gettimeofday(&end, NULL);
    elapsed = (end.tv_sec - start.tv_sec) * 1000000 + (end.tv_usec - start.tv_usec);

    printf("Multi-Thread Execution Time: %" PRIu64 " microseconds\n", elapsed);

    return 0;
}



