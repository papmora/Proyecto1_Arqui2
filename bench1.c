#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <stdint.h>
#include <inttypes.h>
#include <sys/time.h>
#include <sys/sysinfo.h>
#include <unistd.h>

#define ARRAY_SIZE 1000000
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

int systemInformation() {
    // Guardar info en txt
    system("lscpu > lscpu_output.txt");

    // Obtencion del size de una Linea de cache de L1 - Datos
    long l1_cache_line_size = sysconf(_SC_LEVEL1_DCACHE_LINESIZE);
    // Obtencion del size de una Instancia de cache de L1 - Datos
    long l1_cache_size = sysconf(_SC_LEVEL1_DCACHE_SIZE);

    // Validacion de L1 - Datos
    if (l1_cache_line_size == -1 || l1_cache_size == -1) {
        perror("Error al obtener el tamaños de cache");
        return 1;
    }

    // Obtencion del size de una Linea de cache de L1 - Datos
    long l2_cache_line_size = sysconf(_SC_LEVEL2_CACHE_LINESIZE);
    // Obtencion del size de una Instancia de cache de L1 - Datos
    long l2_cache_size = sysconf(_SC_LEVEL2_CACHE_SIZE);

    // Validacion de L1 - Datos
    if (l2_cache_line_size == -1 || l2_cache_size == -1) {
        perror("Error al obtener el tamaños de cache");
        return 1;
    }

    // Obtencion del size de una Linea de cache de L1 - Datos
    long l3_cache_line_size = sysconf(_SC_LEVEL3_CACHE_LINESIZE);
    // Obtencion del size de una Instancia de cache de L1 - Datos
    long l3_cache_size = sysconf(_SC_LEVEL3_CACHE_SIZE);

    // Validacion de L1 - Datos
    if (l3_cache_line_size == -1 || l3_cache_size == -1) {
        perror("Error al obtener el tamaños de cache");
        return 1;
    }

    // Pagina de RAM
    long page_size = sysconf(_SC_PAGESIZE);
    // Cantidad total de RAM en bytes:
    long num_phys_pages = sysconf(_SC_PHYS_PAGES);
    // Conversion:
    long long total_memory = (long long)page_size * num_phys_pages;
    // RAM en Gigas:
    double total_memory_gb = (double)total_memory / (1024 * 1024 * 1024);

    // Obtencion precisa de CPU y Threads
    long num_cores = sysconf(_SC_NPROCESSORS_ONLN);
    long num_threads = sysconf(_SC_NPROCESSORS_CONF);

    printf("-----Especificaciones del sistema-----\n");
    printf("Número de núcleos(Number of Cores): %ld\n", num_cores);
    printf("Número de hilos(Number of Threads): %ld\n", num_threads);
    system("lscpu | grep 'Thread(s) per core'");
    system("lscpu | grep 'Core(s) per socket'");
    printf("Cantidad total de memoria RAM: %.2f GB\n", total_memory_gb);
    // Cache L1:
    printf("Cache L1:\n");
    printf("Tamaño de línea: %ld bytes\n", l1_cache_line_size);
    printf("Tamaño de Instancia: %ld bytes\n", l1_cache_size);
    system("lscpu | grep 'L1d cache'");
    // Cache L2:
    printf("Cache L2:\n");
    printf("Tamaño de línea: %ld bytes\n", l2_cache_line_size);
    printf("Tamaño de Instancia: %ld bytes\n", l2_cache_size);
    system("lscpu | grep 'L2 cache'");
    // Cache L3:
    printf("Cache L3:\n");
    printf("Tamaño de línea: %ld bytes\n", l3_cache_line_size);
    printf("Tamaño de Instancia: %ld bytes\n", l3_cache_size);
    system("lscpu | grep 'L3 cache'");

    printf("Procesador:\n");
    system("lscpu | grep 'Model name'");
}

int main() {
    
    // UwU
    systemInformation();

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



