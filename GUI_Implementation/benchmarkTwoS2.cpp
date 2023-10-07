#include <iostream>
#include <atomic>
#include <thread>
#include <vector>

const int NUM_ITERATIONS = (1 << 27);
const int CACHE_LINE_SIZE = 64;
const int MAX_THREADS = CACHE_LINE_SIZE / sizeof(std::atomic<int>);

// Struct to store counters
struct Counter {
    std::atomic<int> counter;
    Counter() : counter(0) {}
};

// Thread-local variables to hold counters
thread_local Counter tls_counters[MAX_THREADS];

// Valor predeterminado de hilos (- Un solo hilo -)
int NUM_THREADS = 1;

// Valor final
std::atomic<int> finalValue(0);

// Inicialización
void init() {
    for (int i = 0; i < NUM_THREADS; i++) {
        tls_counters[i].counter.store(0);
    }
}

// Función de incremento
void increase(int threadId) {
    int elementsPerThread = NUM_ITERATIONS / NUM_THREADS;

    // Incrementos
    for (int i = 0; i < elementsPerThread; i++) {
        tls_counters[threadId].counter.fetch_add(1);
    }

    std::cout << "Counters[" << threadId << "] - " << &tls_counters[threadId] << " - "<< tls_counters[threadId].counter.load() << std::endl;
    // Suma final
    finalValue.fetch_add(tls_counters[threadId].counter.load());
}

// Impresión
int print() {
    
    std::cout << "Threads: " << NUM_THREADS << std::endl;

    int value = finalValue.load();
    std::cout << "Final Value - " << value << std::endl;

    return 0;
}

int main(int argc, char* argv[]) {
    // Validación
    if (argc != 2) {
        std::cout << "Usage: " << argv[0] << " <num_threads>" << std::endl;
        return 1;
    }

    // Actualizar el número de hilos
    NUM_THREADS = std::atoi(argv[1]);

    if (NUM_THREADS <= 0 || NUM_THREADS > MAX_THREADS) {
        std::cout << "Invalid number of threads. Maximum supported: " << MAX_THREADS << std::endl;
        return 1;
    }

    std::vector<std::thread> threads;
    init();

    std::vector<int> threadIDs(NUM_THREADS);

    // Creación de hilos
    for (int i = 0; i < NUM_THREADS; i++) {
        threadIDs[i] = i;
        threads.emplace_back([i]() { increase(i); });
    }

    for (auto& thread : threads) {
        thread.join();
    }

    print();

    return 0;
}

