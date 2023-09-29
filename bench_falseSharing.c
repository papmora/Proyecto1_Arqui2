#include <stdio.h>
#include <stdatomic.h>
#include <pthread.h>
#include <time.h>

volatile int isRunning = 1;

void* work(void* arg) {
    atomic_int* a = (atomic_int*)arg;
    // for (int i = 0; i < 100000; ++i) {
    //     atomic_fetch_add(a, 1);
    // }
    while (isRunning) {
        atomic_fetch_add(a, 1);
    }
    return NULL;
}

void falseSharing(int durationSeconds) {
    // Create four atomic integers
    atomic_int a = ATOMIC_VAR_INIT(0);
    atomic_int b = ATOMIC_VAR_INIT(0);
    atomic_int c = ATOMIC_VAR_INIT(0);
    atomic_int d = ATOMIC_VAR_INIT(0);

    pthread_t t1, t2, t3, t4;

    // Create four threads, each working on a different atomic variable
    pthread_create(&t1, NULL, work, &a);
    pthread_create(&t2, NULL, work, &b);
    pthread_create(&t3, NULL, work, &c);
    pthread_create(&t4, NULL, work, &d);

    // Esperar durante el tiempo especificado
    sleep(durationSeconds);

    // Detener los hilos
    isRunning = 0;

    // Join the four threads
    pthread_join(t1, NULL);
    pthread_join(t2, NULL);
    pthread_join(t3, NULL);
    pthread_join(t4, NULL);
}


void print() {
    atomic_int a = ATOMIC_VAR_INIT(0);
    atomic_int b = ATOMIC_VAR_INIT(0);
    atomic_int c = ATOMIC_VAR_INIT(0);
    atomic_int d = ATOMIC_VAR_INIT(0);

    printf("Address of atomic_int a - %p\n", (void*)&a);
    printf("Address of atomic_int b - %p\n", (void*)&b);
    printf("Address of atomic_int c - %p\n", (void*)&c);
    printf("Address of atomic_int d - %p\n", (void*)&d);

}

int main() {
    int durationSeconds = 3;
    falseSharing(durationSeconds);
    print();
    return 0;
}

