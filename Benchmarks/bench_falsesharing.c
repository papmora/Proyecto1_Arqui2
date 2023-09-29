#include<stdatomic.h>
#include <pthread.h>

int ITERATIONS = 1<<27;

int MAX_THREADS = 4 ;

int ITERATIONS_PER_THREAD;

atomic_int counters[4]={0,0,0,0};

void incrementCounter(int thread_id)
{
    for(int i = 0; i <ITERATIONS_PER_THREAD;i++)
    {
        counters[thread_id]++;
    }
}

int main()
{
    ITERATIONS_PER_THREAD = ITERATIONS/MAX_THREADS;

    pthread_t threads[MAX_THREADS];

    for(int i = 0; i<MAX_THREADS; i++)
    {
        pthread_create(&threads[i], NULL, (void *(*)(void *)) &incrementCounter, &i);
    }

     for(int i=0; i<MAX_THREADS; i++)
    {
        pthread_join(threads[i],NULL);
    }

    return 0;
}