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
        atomic_int temp = atomic_load(&array[i]);
        atomic_store(&array[i],atomic_load(&array[j]));
        atomic_store(&array[j],temp);

        print(array,MAX_SIZE);
    }

    if(!sorted())
    {
        bogosort();
    }
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

void quicksort(int *limits)
{
    int begin=limits[0];
    int end=limits[1];
    int size=end-begin+1;

    //print(array+begin,size);

    if(size>1)
    {
        int sorted=1;
        int pivot_ind=begin+size/2;
        atomic_int pivot=atomic_load(&array[pivot_ind]);
        int mid_space=0;

        int i = begin;

        while(i<end+1)
        {
            atomic_int element = atomic_load(&array[i]);

            if(i<size-1)
            {

                atomic_int nextElement = atomic_load(&array[i + 1]);

                if(nextElement < element)
                {
                    sorted = 0;
                }
            }
            if(element>=pivot && i<pivot_ind)
            {
                for(int j = i; j<=pivot_ind+mid_space;j++)
                {
                    if(j+1<=end)
                    {
                        atomic_int value = atomic_load(&array[j+1]);
                        atomic_store(&array[j],value);
                    }
                }
                atomic_store(&array[pivot_ind+mid_space],element);
                pivot_ind --;
                i --;
                sorted=0;
                mid_space=mid_space+(element == pivot);
                //print(array+begin,size);
            }
            else if(element < pivot && i>pivot_ind)
            {
                for(int j = i-1;j>=pivot_ind;j--)
                {
                    int value = array[j];
                    array[j+1]=value;
                }
                atomic_store(&array[pivot_ind],element);
                pivot_ind++;
                sorted=0;
                //print(array+begin,size);
            }
            i++;
        }
        if(!sorted)
        {
            int lowerArgs[] = {begin,pivot_ind-1};
            int upperArgs[] = {pivot_ind+mid_space+1,end};

            quicksort(lowerArgs);
            quicksort(upperArgs);

            /*pthread_t lower_thread;
            pthread_t upper_thread;

            pthread_create(&lower_thread, NULL, (void *(*)(void *)) &quicksort, &lowerArgs);
            pthread_create(&upper_thread, NULL, (void *(*)(void *)) &quicksort, &upperArgs);

            /*if(active_threads<MAX_THREADS)
            {
                pthread_create(&lower_thread, NULL, (void *(*)(void *)) &quicksort, &lowerArgs);
                active_threads++;

                if(active_threads<MAX_THREADS)
                {
                    pthread_create(&upper_thread, NULL, (void *(*)(void *)) &quicksort, &upperArgs);
                    active_threads++;
                }

                else
                {
                    quicksort(upperArgs);
                }

            }
            else
            {
                quicksort(lowerArgs);

                if(active_threads<MAX_THREADS)
                {
                    pthread_create(&upper_thread, NULL, (void *(*)(void *)) &quicksort, &upperArgs);
                    active_threads++;
                }

                else
                {
                    quicksort(upperArgs);
                }
            }
            pthread_join(lower_thread,NULL);
            active_threads--;
            pthread_join(upper_thread,NULL);
            active_threads--;*/



        }

    }
}


int main(int argc, char *argv[])
{

    //int arr[MAX_SIZE] = {220, 94, 30, 70, 33, 205, 19, 194, 93, 33, 109, 222, 172, 63, 245, 36};

    //MAX_THREADS=atoi(argv[1]);

    MAX_THREADS =1;
    //MAX_SIZE = MAX_THREADS*16;
    //MAX_SIZE = 1600;
    //MAX_THREADS = atoi(argv[1]);
    //MAX_SIZE = atoi(argv[2]);

    //array= malloc(sizeof(atomic_int)* MAX_SIZE);

    srand(time(NULL));

    for (int i = 0; i<MAX_SIZE;i++)
    {
        int value = 0+rand()%256;
        //atomic_int *value = (atomic_int*)arr[i];
        atomic_store(&array[i],value);
    }

    printf("\nOriginal Array: \n");
    print(array,MAX_SIZE);

    //struct ThreadArgs args ={array,0,15};

    /*int limits[]={0,MAX_SIZE-1};

    quicksort(limits);*/

    pthread_t threads [MAX_THREADS];

    int size = MAX_SIZE/MAX_THREADS;

    for(int i=0; i<MAX_THREADS; i++)
    {

        //int* args = (int*)malloc(2 * sizeof(int));

        //args[0]=i*size;
        //args[1]=i*size+size-1;

        //printf("\nInitial Limits: %d %d\n\n",args[0],args[1]);

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
