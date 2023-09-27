#include <iostream>
#include <vector>
#include <thread>



#define NUM_THREADS 4
#define ARRAY_SIZE 1000000

// Definir una estructura simple para representar un elemento en el arreglo
struct ArrayElement {
    int value;
};

// Arreglo compartido
ArrayElement sharedArray[ARRAY_SIZE];

// Función que realiza la suma en un subconjunto del arreglo
void sumInParallel(int threadId, int start, int end, int &localSum) {
    localSum = 0;

    for (int i = start; i < end; i++) {
        // Realizar una operación de suma en el elemento del arreglo
        localSum += sharedArray[i].value;
    }
}

int main() {
    std::vector<std::thread> threads;
    std::vector<int> threadSums(NUM_THREADS);

    // Inicializar el arreglo compartido con valores
    for (int i = 0; i < ARRAY_SIZE; i++) {
        sharedArray[i].value = 1;  // Inicializar todos los elementos con el mismo valor
    }

    // Dividir el trabajo entre los hilos
    for (int i = 0; i < NUM_THREADS; i++) {
        int start = i * (ARRAY_SIZE / NUM_THREADS);
        int end = (i + 1) * (ARRAY_SIZE / NUM_THREADS);

        threads.emplace_back(sumInParallel, i, start, end, std::ref(threadSums[i]));
    }

    // Esperar a que todos los hilos terminen
    for (std::thread &t : threads) {
        t.join();
    }

    // Calcular el resultado total
    int totalSum = 0;
    for (int sum : threadSums) {
        totalSum += sum;
    }

    // Imprimir el resultado total
    std::cout << "Multi-Threading Sum Result: " << totalSum << std::endl;

    return 0;
}
