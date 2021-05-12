// this file is for you for testing purposes, it won't be included in evaluation.

#include <iostream>
#include <random>
#include <ctime>
#include "the1.h"

using namespace std;


void randomArray(int*& array, int size, int min, int interval)
{

    for (int i = 0; i < size; i++)
    {
        array[i] = min + (random() % interval);
    }
}

void measureTime(){

    clock_t begin;
    clock_t end;
    double duration;

    // maybe data generation here

    if ((begin = clock ()) == -1)
        std::cerr << "clock err" << std::endl;

    // call your function here instead of the loop
    for (int i = 0; i < 1<<30; i++)
        ;
    //

    if ((end = clock ()) == -1)
        std::cerr << "clock err" << std::endl;
    duration = ((double)end-begin) / CLOCKS_PER_SEC * 1000000;

    std::cerr << "Times: " << begin <<" "<< end << " " << end-begin << " "<< duration <<" microsecs " << std::endl;
}


int main()
{
    srandom(time(0));

    int size;
    int *arr;
    int ans;

    printf("=====\nMergeSort\n=======\n");

    printf("=====\nTrial\n=======\n");
    int trial1[] = {12, 11, 13, 5, 6, 7};
    arr = trial1;
    size=6;
    ans = MergeSort(arr,size,true);
    for(int i=0;i<size;i++)
        printf("%d ",arr[i]);
    printf("\n");
    printf("5 6 7 11 12 13");
    printf("\n%d\n",ans);

    printf("=====\nEx1\n====\n");
    int ex1[] = {3,2,7,10};
    arr = ex1;
    size=4;
    ans = MergeSort(arr,size,false);
    for(int i=0;i<size;i++)
        printf("%d ",arr[i]);
    printf("\n");
    printf("10 7 3 2");
    printf("\n%d\n",ans);

    printf("=====\nEx2\n====\n");
    int ex2[] = {-11,2,6,7,-1,5};
    arr = ex2;
    size=6;
    ans = MergeSort(arr,size,true);
    for(int i=0;i<size;i++)
        printf("%d ",arr[i]);
    printf("\n");
    printf("-11 -1 2 5 6 7");
    printf("\n%d\n",ans);

    printf("=====\nEx3\n====\n");
    int ex3[] = {7,1,-2,5,17,8,-5,3,2,-3,0};
    arr = ex3;size=11;
    ans = MergeSort(arr,size,true);
    for(int i=0;i<size;i++)
        printf("%d ",arr[i]);
    printf("\n");
    printf("-5 -3 -2 0 1 2 3 5 7 8 17");
    printf("\n%d\n",ans);


    printf("\n\n\n=====\nFunkySort\n=======\n");

    printf("=====\nTrial\n=======\n");
    int ftrial1[] = {12, 11, 13, 5, 6, 7};
    arr = ftrial1;
    size=6;
    ans = FunkySort(arr,size,true);
    for(int i=0;i<size;i++)
        printf("%d ",arr[i]);
    printf("\n");
    printf("5 6 7 11 12 13");
    printf("\n%d\n",ans);

    printf("=====\nEx1\n====\n");
    int fex1[] = {3,2,7,10};
    arr = fex1;
    size=4;
    ans = FunkySort(arr,size,false);
    for(int i=0;i<size;i++)
        printf("%d ",arr[i]);
    printf("\n");
    printf("10 7 3 2");
    printf("\n%d\n",ans);

    printf("=====\nEx2\n====\n");
    int fex2[] = {-11,2,6,7,-1,5};
    arr = fex2;
    size=6;
    ans = FunkySort(arr,size,true);
    for(int i=0;i<size;i++)
        printf("%d ",arr[i]);
    printf("\n");
    printf("-11 -1 2 5 6 7");
    printf("\n%d\n",ans);

    printf("=====\nEx3\n====\n");
    int fex3[] = {7,1,-2,5,17,8,-5,3,2,-3,0};
    arr = fex3;
    size=11;
    ans = FunkySort(arr,size,true);
    for(int i=0;i<size;i++)
        printf("%d ",arr[i]);
    printf("\n");
    printf("-5 -3 -2 0 1 2 3 5 7 8 17");
    printf("\n%d\n",ans);
    return 0;
}
