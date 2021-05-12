// this file is for you for testing purposes, it won't be included in evaluation.

#include <iostream>
#include <random>
#include <ctime>
#include "the2.h"

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

    int *arr;
    int size;
    int ans;

    printf("=====\nQuickSort\n=====\n");

    printf("=====\nEx1\n=====\n");
    int ex1[] = {-12, -1, 6, 13};
    arr = ex1; size = 4;
    ans = QuickSort(arr, size, true);
    for(int i = 0; i < size; i++)
        printf("%d ",arr[i]);
    printf("\n-12 -1 6 13\n%d\n6\n",ans);

    printf("=====\nEx2\n=====\n");
    int ex2[] = {17, 2, -15, -17, 5, -7};
    arr = ex2; size = 6;
    ans = QuickSort(arr, size, true);
    for(int i = 0; i < size; i++)
        printf("%d ",arr[i]);
    printf("\n-17 -15 -7 2 5 17\n%d\n9\n",ans);

    printf("=====\nEx3\n=====\n");
    int ex3[] = {1, -11, -6, -10, -11, 1, -6, -16};
    arr = ex3; size = 8;
    ans = QuickSort(arr, size, false);
    for(int i = 0; i < size; i++)
        printf("%d ",arr[i]);
    printf("\n1 1 -6 -6 -10 -11 -11 -16\n%d\n20\n",ans);


    printf("=====\nQuickSort3\n=====\n");

    printf("=====\nEx1\n=====\n");
    int ex4[] = {-6, 3, -6, -6, -6, -6, 3, 0, 3};
    arr = ex4; size = 9;
    ans = QuickSort3(arr, size, true);
    for(int i = 0; i < size; i++)
        printf("%d ",arr[i]);
    printf("\n-6 -6 -6 -6 -6 0 3 3 3\n%d\n20\n",ans);

    printf("=====\nEx2\n=====\n");
    int ex5[] = {0, 8, 8, -4, 8, -4, 8, -4, -4, -4, -4};
    arr = ex5; size = 11;
    ans = QuickSort3(arr, size, true);
    for(int i = 0; i < size; i++)
        printf("%d ",arr[i]);
    printf("\n-4 -4 -4 -4 -4 -4 0 8 8 8 8\n%d\n27\n",ans);

    printf("=====\nEx3\n=====\n");
    int ex6[] = {4, 3, 4, 4, 4, 0, 4, 3, 4, 3, 3, 3, 4, 0, 4};
    arr = ex6; size = 15;
    ans = QuickSort3(arr, size, false);
    for(int i = 0; i < size; i++)
        printf("%d ",arr[i]);
    printf("\n4 4 4 4 4 4 4 4 3 3 3 3 3 0 0\n%d\n42\n",ans);

    printf("=====\nEx4\n=====\n");
    int ex7[] = {-3, 1, -11, 1, 3, 0, -12, -3, 4, -12, -3, -7, 5, 0, 5, -6, 6, 4, -4, 4, -11, 0, -12, -8, -3, -8, -1, -2};
    arr = ex7; size = 28;
    ans = QuickSort3(arr, size, true);
    for(int i = 0; i < size; i++)
        printf("%d ",arr[i]);
    printf("\n%d\n",ans);


    printf("=====\nEx4\n=====\n");
    int ex8[] = {2, 2, 2, 2, 2, 2, 2, 2, 2, 2};
    arr = ex8; size = 10;
    ans = QuickSort(arr, size, true);
    for(int i = 0; i < size; i++)
        printf("%d ",arr[i]);
    printf("\n%d\n",ans);

    printf("=====\nEx4\n=====\n");
    int ex9[] = {1};
    arr = ex9; size = 1;
    ans = QuickSort(arr, size, true);
    for(int i = 0; i < size; i++)
        printf("%d ",arr[i]);
    printf("\n%d\n",ans);

    printf("=====\nEx4\n=====\n");
    int ex10[] = {1, 1};
    arr = ex10; size = 2;
    ans = QuickSort(arr, size, true);
    for(int i = 0; i < size; i++)
        printf("%d ",arr[i]);
    printf("\n%d\n",ans);

    printf("=====\nEx4\n=====\n");
    int ex11[] = {1, 1, 1};
    arr = ex11; size = 3;
    ans = QuickSort(arr, size, true);
    for(int i = 0; i < size; i++)
        printf("%d ",arr[i]);
    printf("\n%d\n",ans);

    printf("=====\nEx4\n=====\n");
    int ex12[] = {1, 2, 3};
    arr = ex12; size = 3;
    ans = QuickSort(arr, size, true);
    for(int i = 0; i < size; i++)
        printf("%d ",arr[i]);
    printf("\n%d\n",ans);

    return 0;
}
