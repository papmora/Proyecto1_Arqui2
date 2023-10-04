#include <stdio.h>
#include <stdlib.h>
#include <stdatomic.h>
#include <pthread.h>

#define MAX_SIZE 8

#define MAX_THREADS  8

#define CACHE_LINE_SIZE 64

atomic_int array[CACHE_LINE_SIZE/sizeof(atomic_int)];

int enables[MAX_SIZE]={0};

int finishes[MAX_SIZE]={0};

int ends[MAX_SIZE]={0};

struct ThreadArgs
{
    int thread_id;
};

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

void modifyValue(void* arg)
{

    struct ThreadArgs *args = (struct ThreadArgs *)arg;

    int thread_id=args->thread_id;

    while(!finishes[thread_id])
    {

        if(enables[thread_id])
        {
            atomic_int val = array[thread_id];

            int end=ends[thread_id];

            if(val!=end)
            {
                int direction = (end-val)/abs(end-val);

                while(val!=end)
                {
                    //printf("%d\n",*array[ind]);
                    if(direction == 1)
                    {
                        array[thread_id]++;
                    }
                    else
                    {
                        array[thread_id]--;
                    }
                    val = array[thread_id];
                }
            }
            enables[thread_id]=0;
            ends[thread_id]=0;
        }
    }

}

void swap(int ind1, int ind2)
{

    atomic_int val1 = (atomic_int)array[ind1];
    atomic_int val2 = (atomic_int)array[ind2];

    ends[ind1]=val2;
    enables[ind1]=1;

    ends[ind2]=val1;
    enables[ind2]=1;
}

void bubblesort()
{
    while(1)
    {
        int sorted = 1;
        for (int i = 0; i<MAX_SIZE-1; i++)
        {
            if(array[i]>array[i+1])
            {
                if(!enables[i] && !enables[i+1])
                {
                    while (1) {
                        if (!enables[i]) {
                            swap(i, i + 1);
                            sorted = 0;
                            break;
                        }
                    }
                }
            }
        }
        if(sorted)
        {
            for(int i = 0; i < MAX_SIZE-1 ; i++)
            {
                if(enables[i] || array[i]>array[i+1])
                {
                    sorted=0;
                    break;
                }
            }
            if(!enables[MAX_SIZE-1] && sorted)
            {
                break;
            }
        }
    }

}

int main(int argc, char *argv[])
{

    printf("TOTAL THREADS: %d\n\n",MAX_THREADS);

    pthread_t threads[MAX_THREADS];

    srand(time(NULL));

    for (int i = 0; i<MAX_SIZE;i++)
    {
        int value = 16777215+rand()%268435456;

        atomic_init(&array[i],value);

        printf("Address of ARRAY[%d]: %p\n",i,&array[i]);
    }

    printf("[");

    for (int i = 0; i < MAX_SIZE; i++)
    {
        atomic_int value = array[i];

        if (i == MAX_SIZE - 1) {
            printf("%d]\n\n", value);
        } else {
            printf("%d, ", value);
        }
    }

    struct ThreadArgs args[MAX_THREADS];

    for(int i = 0; i<MAX_THREADS; i++)
    {
        args[i].thread_id=i;
        pthread_create(&threads[i], NULL, (void *(*)(void *)) &modifyValue,&args[i]);
    }

    bubblesort();

    for(int i=0; i<MAX_THREADS; i++)
    {
        finishes[i]=1;
        pthread_join(threads[i],NULL);
    }

    printf("[");

    for (int i = 0; i < MAX_SIZE; i++)
    {
        atomic_int value = array[i];

        if (i == MAX_SIZE - 1) {
            printf("%d]\n\n", value);
        } else {
            printf("%d, ", value);
        }
    }

    return 0;
}