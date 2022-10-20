#include <stdio.h>
#include <string.h>
#include <time.h>
#include <sys/time.h>
#include <stdlib.h>
#include "mymalloc.h"

/* A: malloc() 1 byte and immediately free it - do this 120 times */
int Test1()
{
    struct timeval start, finish;
    gettimeofday(&start, NULL);
    
    int i = 0;
    while (i < 120)
    {
        char *a = malloc(1); // Malloc() 1 byte then immediately free()
        free(a);
        i++;
    }
    
    gettimeofday(&finish, NULL);
    
    return (int)((finish.tv_sec - start.tv_sec) / 10000000 + (finish.tv_usec - start.tv_usec));
}


int Test2()
{
    struct timeval start, finish;
    gettimeofday(&start, NULL);
    
    char *array[1];
    int k = 0;
    int i = 0;
    int j = 0;
    
    
    while (i < 120) 
    {
        while (j < 1) 
        {
            array[j] = malloc(1);
            j += 1;
        }
        j = 0;
               
        while (k < 1)
        {
            free(array[k]);
            k += 1;
        }
        k = 0;
        
        i += 1;
    }
    
    gettimeofday(&finish, NULL);
    return (int)((finish.tv_sec - start.tv_sec) / 10000000 + (finish.tv_usec - start.tv_usec));
}


int test3()
{
    struct timeval start, finish;
    gettimeofday(&start, NULL);
    
    int boolean;           
    int count = 0; 
    int numPointers = 0;  
    int *temp[120];        
    
    unsigned long int randomNumber = time(NULL);
    
    while (count < 120) 
    {
        randomNumber = rand();
        boolean = randomNumber % 2;
        if (boolean) 
        {
            count += 1;            
            temp[numPointers] = malloc(1); 
            numPointers += 1;               
        }
        else 
        {
            if (numPointers > 0) 
            {
                free(temp[numPointers - 1]); 
                numPointers -= 1;            
            }
        }
    }
    
 
    while (numPointers > 0)
    {
        free(temp[numPointers - 1]); 
        numPointers -= 1;             
    }
    
    gettimeofday(&finish, NULL);
    return (int)((finish.tv_sec - start.tv_sec) / 10000000 + (finish.tv_usec - start.tv_usec));
}




int Test4()
{
    struct timeval start, finish;
    gettimeofday(&start, NULL);
    int count=0;
    while(count<120){
        int i;
        int *p = malloc(5* sizeof(int));
        for (i=0; i<5;i++){
            p[i]=i;
        }
        free(p);
        count++;
    }
    
    gettimeofday(&finish, NULL);
    return (int)((finish.tv_sec - start.tv_sec) /10000000 + (finish.tv_usec - start.tv_usec));
}


int Test5()
{
    
    struct timeval start, finish;
    gettimeofday(&start, NULL);
    
    int *z = (int*) malloc(200);
    free(z);
    z = (int*) malloc(200);
    free(z);
    
    gettimeofday(&finish, NULL);
    return (int)((finish.tv_sec - start.tv_sec) / 10000000 + (finish.tv_usec - start.tv_usec));
}

int main(int argc, const char *argv[])
{
    float countTest1 = 0.0; 
    float countTest2 = 0.0;
    float countTest3 = 0.0;
    
    float countTest4 = 0.0;
    float countTest5 = 0.0;
    
    int i;
    for (i = 0; i < 50; ++i)
    {
        countTest1 += Test1();
        countTest2 += Test2();
        countTest3 += test3();
        
        countTest4 += Test4();
        countTest5 += Test5();
    }
    
    printf("AFTER 50 TESTS:\n"); 
    printf("TEST 1 AVERAGE: %.2f microseconds\n",  countTest1 / 50);
    printf("TEST 2 AVERAGE: %.2f microseconds\n", countTest2 / 50);
    printf("TEST 3 AVERAGE: %.2f microseconds\n",  countTest3 / 50);
    
    printf("TEST 4 AVERAGE: %.2f microseconds\n",  countTest4/50 );
    printf("TEST 5 AVERAGE: %.2f microseconds\n",  countTest5 / 50);
    
    return 0;
}