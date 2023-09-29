#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <pthread.h>
#include <stdatomic.h>

int MAX_SIZE;

int MAX_THREADS;

atomic_int *array;

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
void slowsort(int *limits)
{
    int begin=limits[0];
    int end=limits[1];

    int size= end-begin+1;

    atomic_int initial= atomic_load(&array[begin]);
    atomic_int last= atomic_load(&array[end]);

    if(initial>last)
    {
        atomic_int temp=initial;
        initial=last;
        last=temp;

        atomic_store(&array[begin],initial);
        atomic_store(&array[end],last);
    }

    if(size>2)
    {
        int limit = size/3;

        int initialTwoThirdsArgs[] = {begin,end-limit};
        int finalTwoThirdsArgs[] = {begin+limit,end};

        slowsort(initialTwoThirdsArgs);
        slowsort(finalTwoThirdsArgs);
        slowsort(initialTwoThirdsArgs);
    }
}


int main(int argc, char *argv[])
{

    
    MAX_THREADS = atoi(argv[1]);
    MAX_SIZE = atoi(argv[2]);

    array= malloc(sizeof(atomic_int)* MAX_SIZE);

    srand(time(NULL));

    for (int i = 0; i<MAX_SIZE;i++)
    {
        int value = 0+rand()%256;
        atomic_store(&array[i],value);
    }

    pthread_t threads [MAX_THREADS];

    int size = MAX_SIZE/MAX_THREADS;

    for(int i=0; i<MAX_THREADS; i++)
    {

        int* args = (int*)malloc(2 * sizeof(int));

        args[0]=i*size;
        args[1]=i*size+size-1;

        pthread_create(&threads[i], NULL, (void *(*)(void *)) &slowsort, args);
    }

    for(int i=0; i<MAX_THREADS; i++)
    {
        pthread_join(threads[i],NULL);
    }

    return 0;
}
