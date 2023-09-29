#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <pthread.h>
#include <stdatomic.h>

int MAX_SIZE;

int MAX_THREADS;

#define CACHE_LINE_SIZE 64

#define MAX_SIZE 8


atomic_int array[2 * CACHE_LINE_SIZE / sizeof(atomic_int)] __attribute__((aligned(CACHE_LINE_SIZE)));

void print(atomic_int *array_p,int size)
{
    if(size<=0)
    {
        printf("[]\n\n");
    }
    else
    {
        printf("[");

        for (int i = 0; i < size; i++) {
            atomic_int value = atomic_load(&array_p[i]);

            if (i == size - 1) {
                printf("%d]\n\n", value);
            } else {
                printf("%d, ", value);
            }
        }

    }
    fflush(stdout);
}

int sorted()
{
    for(int i =0; i<MAX_SIZE-1;i++)
    {
        if(atomic_load(&array[i+1])<atomic_load(&array[i]))
        {
            return 0;
        }
    }
    return 1;
}

void bogosort()
{
    for(int i = 0; i<MAX_SIZE; i++ )
    {
        int j = rand() % MAX_SIZE;
        int temp = atomic_load(&array[i]);
        int swap_value=atomic_load(&array[j]);
        atomic_store(&array[i],swap_value);
        atomic_store(&array[j],temp);

        //print(array,MAX_SIZE);
    }

    if(!sorted())
    {
        bogosort();
    }
}

int main(int argc, char *argv[])
{

    MAX_THREADS =atoi(argv[1]);

    printf("USING %d THREADS \n\n",MAX_THREADS);
   
    srand(time(NULL));

    for (int i = 0; i<MAX_SIZE;i++)
    {
        int value = 0+rand()%256;
        atomic_store(&array[i],value);
    }

    printf("\nOriginal Array: \n");
    print(array,MAX_SIZE);


    pthread_t threads [MAX_THREADS];

    for(int i=0; i<MAX_THREADS; i++)
    {
        pthread_create(&threads[i], NULL, (void *(*)(void *)) &bogosort, NULL);
    }

    for(int i=0; i<MAX_THREADS; i++)
    {
        pthread_join(threads[i],NULL);
    }

    printf("Sorted Array: \n");
    print(array,MAX_SIZE);

    return 0;
}