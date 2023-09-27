#include <stdio.h>
#include <stdatomic.h>
#include <stdlib.h>
#include <time.h>


void print(atomic_int *array[],int size)
{
    printf("[");

    for (int i = 0; i<size ; i++)
    {
        atomic_int value = atomic_load(&array[i]);

        if(i == size - 1)
        {
            printf("%d ]\n\n",value);
        }
        else
        {
            printf("%d, ",value);
        }
    }
    fflush(stdout);
}
void quicksort(atomic_int *array[],int size)
{
    if(size>2)
    {
        int sorted=1;
        int pivot_ind=size/2;
        int pivot=array[pivot_ind];
        int mid_space=0;

        int i = 0;

        while(i<size)
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
                    if(j+1<size)
                    {
                        atomic_int value = atomic_load(&array[j+1]);
                        atomic_store(&array[j],value);
                    }
                }
                atomic_store(&array[pivot_ind],element);
                pivot_ind --;
                i --;
                sorted=0;
                mid_space=mid_space+(element == pivot);
            }
            else if(element < pivot && i>pivot_ind)
            {
                for(int j = i-1;j>=pivot_ind;j--)
                {
                    atomic_int *value = atomic_load(&array[j]);
                    atomic_store(&array[j+1],value);
                }
                atomic_store(&array[pivot_ind],element);
                pivot_ind++;
                sorted=0;
            }
            i++;
        }
        if(!sorted)
        {
            quicksort(array,pivot_ind+1);
            quicksort(array+pivot_ind+mid_space,size-(pivot_ind+mid_space));
        }

    }
}


int main()
{
    int size = 64;

    atomic_int *array[64];

    srand(time(NULL));

    for (int i = 0; i<size;i++)
    {
        atomic_init(&array[i],0+rand()%256);
    }

    printf("Original Array: \n");
    print(array,size);

    quicksort(array,size);

    printf("Sorted Array: \n");
    print(array,size);
    return 0;
}
