#include "the2.h"

// do not add extra libraries here


/*
    arr       : array to be sorted, in order to get points this array should contain be in sorted state before returning
    size      : size of arr
    ascending : true for ascending, false for descending

    you can use ceil function from cmath

*/

void Swap(int*& arr, int i, int j)
{
    int temp = arr[i];
    arr[i] = arr[j];
    arr[j] = temp;
}

int QuickSortHelper (int*& arr, int start, int end, bool ascending)
{
    int result = 0;

    if (start < end) {
        int k = start;

        for (int i = start; i < end; i++) {
            result++;

            if ((ascending && arr[i] < arr[end]) || (!ascending && arr[i] > arr[end])) {
                Swap(arr, i ,k);

                k++;
            }
        }

        Swap(arr, k, end);

        result += QuickSortHelper(arr, start, k - 1, ascending);
        result += QuickSortHelper(arr, k + 1, end, ascending);
    }

    return result;
}

int QuickSort (int*& arr, int size, bool ascending)
{
    return QuickSortHelper(arr, 0, size - 1, ascending);
}

int QuickSort3Helper (int*& arr, int start, int end, bool ascending)
{
    int result = 0;

    if (start < end) {
        int i = start;
        int k = start;
        int p = end;

        while (i < p) {
            if ((ascending && arr[i] < arr[end]) || (!ascending && arr[i] > arr[end])) {
                result += 1;
                Swap(arr, i++, k++);
            } else if (arr[i] == arr[end]) {
                result += 2;
                Swap(arr, i, --p);
            } else {
                result += 2;
                i += 1;
            }
        }

        int m = (p-k < end-p+1) ? (p-k) : (end-p+1);

        for (int j = 0; j <= m - 1; j++) {
            Swap(arr, k + j, end - m + 1 + j);
        }

        result += QuickSort3Helper(arr, start, k - 1, ascending);
        result += QuickSort3Helper(arr, end - p + k + 1, end, ascending);
    }

    return result;
}

int QuickSort3 (int*& arr, int size, bool ascending)
{
    return QuickSort3Helper(arr, 0, size - 1, ascending);
}
