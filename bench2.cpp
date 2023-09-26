#include <iostream>
#include <vector>
#include <thread>
#include <chrono>
#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <sys/sysinfo.h>
#include <unistd.h>

constexpr int ARRAY_SIZE = 1000000;
constexpr int NUM_THREADS = 4;

// Alineación de 64 bytes para provocar false sharing
constexpr int CACHE_LINE_SIZE = 64;

struct alignas(CACHE_LINE_SIZE) CacheLine {
    int data;
};

CacheLine sharedArray[ARRAY_SIZE];

// Función que calcula la suma de elementos en el arreglo compartido
void sumArray(int threadId, int &localSum) {
    localSum = 0;
    for (int i = threadId; i < ARRAY_SIZE; i += NUM_THREADS) {
        // Provocar false sharing: Acceder y sumar elementos cercanos en la memoria.
        localSum += sharedArray[i].data;
    }
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

    return 0;
}

int main() {

    // UwU
    systemInformation();

    std::vector<std::thread> threads;
    std::vector<int> threadSums(NUM_THREADS);
    auto start = std::chrono::high_resolution_clock::now();

    // Inicializar el arreglo compartido con valores
    for (int i = 0; i < ARRAY_SIZE; i++) {
        sharedArray[i].data = 1;  // Inicializar todos los elementos con el mismo valor
    }

    // Medir el tiempo de ejecución de la versión single-thread
    for (int i = 0; i < NUM_THREADS; i++) {
        sumArray(i, threadSums[i]);
    }

    int totalSum = 0;
    for (int sum : threadSums) {
        totalSum += sum;
    }

    auto end = std::chrono::high_resolution_clock::now();
    auto elapsed = std::chrono::duration_cast<std::chrono::microseconds>(end - start).count();

    std::cout << "Single-Thread Sum: " << totalSum << ", Execution Time: " << elapsed << " microseconds\n";

    // Reinicializar el arreglo compartido
    for (int i = 0; i < ARRAY_SIZE; i++) {
        sharedArray[i].data = 1;  // Restaurar valores iniciales
    }

    // Medir el tiempo de ejecución de la versión multi-thread
    start = std::chrono::high_resolution_clock::now();

    for (int i = 0; i < NUM_THREADS; i++) {
        threads.emplace_back(sumArray, i, std::ref(threadSums[i]));
    }

    for (std::thread &t : threads) {
        t.join();
    }

    totalSum = 0;
    for (int sum : threadSums) {
        totalSum += sum;
    }

    end = std::chrono::high_resolution_clock::now();
    elapsed = std::chrono::duration_cast<std::chrono::microseconds>(end - start).count();

    std::cout << "Multi-Thread Sum: " << totalSum << ", Execution Time: " << elapsed << " microseconds\n";

    return 0;
}
