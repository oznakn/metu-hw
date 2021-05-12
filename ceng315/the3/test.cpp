// this file is for you for testing purposes, it won't be included in evaluation.

#include <iostream>
#include <random>
#include <ctime>
#include "the3.h"

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

    long *arr;
    int ans;
    int size;

    printf("=====\nEx1\n=====\n");
    long ex1[] = {454, 435, 8, 23, 68};
    arr = ex1; size = 5;
    ans = RadixSort(arr, true, size, 1);
    for(int i = 0; i < size; i++)
        printf("%ld ",arr[i]);
    printf("\n8 23 68 435 454\n%d\n288\n",ans);

    printf("=====\nEx2\n=====\n");
    long ex2[] = {454, 435, 8, 23, 68};
    arr = ex2; size = 5;
    ans = RadixSort(arr, true, size, 2);
    for(int i = 0; i < size; i++)
        printf("%ld ",arr[i]);
    printf("\n8 23 68 435 454\n%d\n684\n", ans);

    printf("=====\nEx3\n=====\n");
    long ex3[] = {454, 435, 8, 23, 68};
    arr = ex3; size = 5;
    ans = RadixSort(arr, true, size, 4);
    for(int i = 0; i < size; i++)
        printf("%ld ",arr[i]);
    printf("\n8 23 68 435 454\n%d\n30042\n", ans);

    printf("=====\nEx4\n=====\n");
    long ex4[] = {551989238, 396808766, 113775846, 186707985, 444194547};
    arr = ex4; size = 5;
    ans = RadixSort(arr, false, size, 4);
    for(int i = 0; i < size; i++)
        printf("%ld ",arr[i]);
    printf("\n551989238 444194547 396808766 186707985 113775846\n%d\n30042\n", ans);


    printf("=====\nEx5\n=====\n");
    long ex5[] = {551989238, 396808766, 113775846, 186707985, 444194547};
    arr = ex5; size = 5;
    ans = RadixSort(arr, false, size, 6);
    for(int i = 0; i < size; i++)
        printf("%ld ",arr[i]);
    printf("\n551989238 444194547 396808766 186707985 113775846\n%d\n2000028\n", ans);

    return 0;
}
