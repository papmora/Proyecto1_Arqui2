#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

#define NUM_THREADS 4
#define ARRAY_SIZE 1000000

// Definir una estructura simple para representar un elemento en el arreglo
struct ArrayElement {
    int value;
};

// Arreglo compartido
struct ArrayElement sharedArray[ARRAY_SIZE];

// Función que realiza la suma en un subconjunto del arreglo
void *sumInParallel(void *threadId) {
    int id = *(int *)threadId;
    int start = id * (ARRAY_SIZE / NUM_THREADS);
    int end = (id + 1) * (ARRAY_SIZE / NUM_THREADS);
    int localSum = 0;

    for (int i = start; i < end; i++) {
        // Realizar una operación de suma en el elemento del arreglo
        localSum += sharedArray[i].value;
    }

    return (void *)localSum;
}

int main() {
    pthread_t threads[NUM_THREADS];
    int threadIds[NUM_THREADS];

    // Inicializar el arreglo compartido con valores
    for (int i = 0; i < ARRAY_SIZE; i++) {
        sharedArray[i].value = 1;  // Inicializar todos los elementos con el mismo valor
    }

    // Crear hilos y realizar la suma en paralelo
    for (int i = 0; i < NUM_THREADS; i++) {
        threadIds[i] = i;
        pthread_create(&threads[i], NULL, sumInParallel, &threadIds[i]);
    }

    int totalSum = 0;

    // Esperar a que todos los hilos terminen y obtener los resultados locales
    for (int i = 0; i < NUM_THREADS; i++) {
        void *localResult;
        pthread_join(threads[i], &localResult);
        totalSum += (int)localResult;
    }

    // Imprimir el resultado total
    printf("Multi-Threading Sum Result: %d\n", totalSum);

    return 0;
}
