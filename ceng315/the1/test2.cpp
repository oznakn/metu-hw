// this file is for you for testing purposes, it won't be included in evaluation.

#include <iostream>
#include <random>
#include <ctime>
#include "the1.h"

using namespace std;


void printArr(int*& arr, int len)
{
     for(int i = 0; i < len; i++){
        std::cout << arr[i] <<" ";
    }std::cout<< std::endl;
}

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


 // GIVEN TEST CASES SETUP
    int case6[] = { 10, 1, 21, 13, 34, 16, 17, 71, 48, 39, 90,15, 26, 37, 78, 29, 100};
  int size6 = 17;
  int *case6arr = new int[size6];


  int case5[] = { 10, 9, 8, 7, 6, 5, 4, 3, 2, 1, 0};
  int size5 = 11;
  int *case5arr = new int[size5];



  int case4[] = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
  int size4 = 11;
  int *case4arr = new int[size4];


  int case3[] = { 7, 1, -2, 5, 17, 8, -5, 3, 2, -3, 0};
  int size3 = 11;
  int *case3arr = new int[size3];

 int case2[] = { -11, 2, 6, 7, -1, 5 };
 int size2 = 6;
 int *case2arr= new int[size2];

 int case1[] = {3,2,7,10};
 int size1= 4;
 int *case1arr = new int[size1];







  for(int i = 0 ; i < size6; i++){
     case6arr[i] = case6[i];
 }



  for(int i = 0 ; i < size5; i++){
     case5arr[i] = case5[i];
 }
  for(int i = 0 ; i < size4; i++){
     case4arr[i] = case4[i];
 }

 for(int i = 0 ; i < size3; i++){
     case3arr[i] = case3[i];
 }
 for(int i = 0 ; i < size2; i++){
     case2arr[i] = case2[i];
 }
 for(int i = 0 ; i < size1; i++){
     case1arr[i] = case1[i];
 }









// GIVEN TEST CASES SETUP



    // MERGE SORT TEST CASES
    std::cout <<"Merge 1:"<< MergeSort(case1arr,size1,false) << std::endl;
    printArr(case1arr,size1);
    std::cout <<"Merge 2:"<< MergeSort(case2arr,size2,true) << std::endl;
    printArr(case2arr,size2);
    std::cout <<"Merge 3:"<< MergeSort(case3arr,size3,true) << std::endl;
    printArr(case3arr,size3);
    std::cout <<"Merge 4:"<< MergeSort(case4arr,size4,true) << std::endl;
    printArr(case4arr,size4);
    std::cout <<"Merge 5:"<< MergeSort(case5arr,size5,true) << std::endl;
    printArr(case5arr,size5);
    std::cout <<"Merge 6:"<< MergeSort(case6arr,size6,true) << std::endl;
    printArr(case6arr,size6);



    // MERGE SORT TEST CASES








    // RESET ARRAYS FOR REUSE
     for(int i = 0 ; i < size6; i++){
         case6arr[i] = case6[i];
     }


    for(int i = 0 ; i < size5; i++){
         case5arr[i] = case5[i];
     }

    for(int i = 0 ; i < size4; i++){
         case4arr[i] = case4[i];
     }

     for(int i = 0 ; i < size3; i++){
         case3arr[i] = case3[i];
     }
     for(int i = 0 ; i < size2; i++){
         case2arr[i] = case2[i];
     }
     for(int i = 0 ; i < size1; i++){
         case1arr[i] = case1[i];
     }
    // RESET ARRAYS FOR REUSE











    // FUNKY SORT TEST CASES
    std::cout <<"Count 1:"<< FunkySort(case1arr,size1,false) << std::endl;
    printArr(case1arr,size1);
    std::cout <<"Count 2:"<< FunkySort(case2arr,size2,true) << std::endl;
    printArr(case2arr,size2);
    std::cout <<"Count 3:"<< FunkySort(case3arr,size3,true) << std::endl;
    printArr(case3arr,size3);
    std::cout <<"Count 4:"<< FunkySort(case4arr,size4,true) << std::endl;
    printArr(case4arr,size4);
    std::cout <<"Count 5:"<< FunkySort(case5arr,size5,true) << std::endl;
    printArr(case5arr,size5);
    std::cout <<"Count 6:"<< FunkySort(case6arr,size6,true) << std::endl;
    printArr(case6arr,size6);


    // FUNKY SORT TEST CASES

    std::cout<< std::endl;


// RANDOM LARGE INPUT
    int size = 100000; //
    int* randArr = new int[size];
    randomArray(randArr, size, -2000,2000);
// RANDOM LARGE INPUT

 // TIMED TEST ZONE
  if ((begin = clock ()) == -1)
    std::cerr << "clock err" << std::endl;

        // std::cout <<"Count large:  "<< FunkySort(randArr,size,false) << std::endl;
        std::cout <<"Merge large: "<< MergeSort(randArr,size,false) << std::endl;

  if ((end = clock ()) == -1)
    std::cerr << "clock err" << std::endl;
  duration = ((double)end-begin) / CLOCKS_PER_SEC; //* 1000000;

  std::cerr << "Time: " << duration <<" secs " << std::endl;

 // TIMED TEST ZONE
}




int main()
{
    srandom(time(0));
    measureTime();
  return 0;
}
