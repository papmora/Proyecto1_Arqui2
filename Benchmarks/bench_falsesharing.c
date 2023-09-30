#include<stdatomic.h>
#include <pthread.h>
#include <stdio.h>

#define MAX_SIZE 8

#define MAX_THREADS  8

#define CACHE_LINE_SIZE 64

int ITERATIONS = 1<<27;

int ITERATIONS_PER_THREAD;

atomic_int counters[8];

struct ThreadArgs 
{
    int thread_id;
};

void incrementCounter(void* arg)
{

    struct ThreadArgs *args = (struct ThreadArgs *)arg;

    int thread_id=args->thread_id;

    for(int i = 0; i <ITERATIONS_PER_THREAD;i++)
    {
        counters[thread_id] ++;
    }
}

int main()
{
    ITERATIONS_PER_THREAD = ITERATIONS/MAX_THREADS;

    pthread_t threads[MAX_THREADS];

    struct ThreadArgs args[MAX_THREADS];


    for(int i = 0; i<MAX_THREADS; i++)
    {
        printf("Address of ARRAY[0]: %p\n",&counters[i]);
        args[i].thread_id=i;
        pthread_create(&threads[i], NULL, (void *(*)(void *)) &incrementCounter, &args[i]);
    }

     for(int i=0; i<MAX_THREADS; i++)
    {
        pthread_join(threads[i],NULL);
    }

    return 0;
}