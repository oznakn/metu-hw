// this file is for you for testing purposes, it won't be included in evaluation.

#include <iostream>
#include <random>
#include <ctime>
#include <limits>
#include "the3.h"



using namespace std;





long intPowerTen(int n);
bool isArraySorted(long* arr, int size, bool asc);
void printArrayLong(const long* arr, int &size);

void randomArray(long*& array, int size, int min, int interval)
{

  for (int i = 0; i < size; i++)
  {
    array[i] = min + 464*(random() % interval);
  }
}

void measureTime(long*& arr, int size, bool asc, int& compCount, int l){

    clock_t begin;
    clock_t end;
    double duration;

//    puts("A");
    randomArray(arr, size, 0, intPowerTen(12)-1);
//    puts("B");

    // maybe data generation here

     if ((begin = clock ()) == -1)
    std::cerr << "clock err" << std::endl;

    // call your function here instead of the loop
    compCount = RadixSort(arr, asc, size, l);
    for (int i = 0; i < 1<<30; i++)
        ;
    //

    if ((end = clock ()) == -1)
    std::cerr << "clock err" << std::endl;
    duration = ((double)end-begin) / CLOCKS_PER_SEC * 1000000;

    std::cerr << "Times: " << begin <<" "<< end << " " << end-begin << " "<< duration <<" microsecs " << std::endl;


}


bool isArraySorted(long* arr, int size, bool asc)
{
    if (asc)
    {
        for (int i = 0  ; i < size-1 ; i++)
        {
            if (arr[i] > arr[i+1])
                return false;
        }
        return true;
    }
    else
    {
        for (int i = 0  ; i < size-1 ; i++)
        {
            if (arr[i] < arr[i+1])
                return false;
        }
        return true;
    }

    return false;
}



long intPowerTen(int n)
{
    long res=1;

    if (n == 0) return 1;

    for(int i =0 ; i< n  ; i++)
        res *= 10;

    return res;
}

void printArrayLong(const long* arr, int &size)
{
    for (int i = 0  ; i < size ; i++)
    {
        printf("%ld ", arr[i]);
    }
    printf("\n");
}



//int RadixSort(long arr[], bool ascending, int n, int l )

int main()
{
    srandom(time(0));
    int compCount=0;

    bool asc = true; // !!!!MAKE IT FALSE TO TEST DESC CASE!!!!
    int size;
    long* arr;
    int l[] = {1, 2, 3, 4, 6};
    int x;


    size = intPowerTen(3);
    arr = new long[size];
    for (int i = 0 ; i < 5 ; i++)
    {
        printf("===========n:%6d ========= l:%2d=========\n",size, l[i]);
        measureTime(arr, size, asc, compCount, l[i]);
        x = (3*size + intPowerTen(l[i])-1)*(12/l[i]);
        printf("Program returns: %d\n", compCount);
        printf("Calculated count: %d\n", x);
        if(isArraySorted(arr, size, asc))
        {
            printf("Array is sorted\n");
        }
        else
        {
            printf("Array is not sorted\n");
            printArrayLong(arr, size);
        }
        puts("");

    }
    delete[] arr;


    size = intPowerTen(6);
    arr = new long[size];
    for (int i = 0 ; i < 5 ; i++)
    {
        printf("===========n:%6d ========= l:%2d=========\n",size, l[i]);
        measureTime(arr, size, asc, compCount, l[i]);
        x = (3*size + intPowerTen(l[i])-1)*(12/l[i]);
        printf("Program returns: %d\n", compCount);
        printf("Calculated count: %d\n", x);
        if(isArraySorted(arr, size, asc))
        {
            printf("Array is sorted\n");
        }
        else
        {
            printf("Array is not sorted\n");
            printArrayLong(arr, size);
        }
        puts("");

    }
    delete[] arr;



    return 0;
}
