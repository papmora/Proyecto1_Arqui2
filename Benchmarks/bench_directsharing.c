#include<stdatomic.h>
#include <pthread.h>
#include <stdio.h>

int ITERATIONS = 1<<27;

int MAX_THREADS = 4 ;

int ITERATIONS_PER_THREAD;

atomic_int counter;

void incrementCounter()
{
    for(int i = 0; i <ITERATIONS_PER_THREAD;i++)
    {
        atomic_store(&counter,counter+1);
    }
}

int main()
{
    ITERATIONS_PER_THREAD = ITERATIONS/MAX_THREADS;

    counter = 0;

    pthread_t threads[MAX_THREADS];

    printf("Address of ITERATIONS_PER_THREAD %p\n", (void*)&ITERATIONS_PER_THREAD);
    printf("Address of COUNTER %p\n\n", (void*)&counter);

    for(int i = 0; i<MAX_THREADS; i++)
    {
        pthread_create(&threads[i], NULL, (void *(*)(void *)) &incrementCounter, NULL);
    }

     for(int i=0; i<MAX_THREADS; i++)
    {
        pthread_join(threads[i],NULL);
    }

    return 0;

}