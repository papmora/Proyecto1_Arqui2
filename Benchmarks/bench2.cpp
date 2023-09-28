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

int main() {

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
